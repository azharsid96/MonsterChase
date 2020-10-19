
#include "HeapManager.h"
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#define __TRACK_ALLOCATIONS

HeapManagerProxy::HeapManager* heapManagerInstance;

namespace HeapManagerProxy
{
	HeapManager * CreateHeapManager(void * i_pMemory, size_t i_sizeMemory)
	{
		return HeapManager::create(i_pMemory, i_sizeMemory);
	}

	void Destroy(HeapManager * i_pManager)
	{
		assert(i_pManager);

		i_pManager->destroy();
	}

	void * alloc(HeapManager * i_pManager, size_t i_size)
	{
		assert(i_pManager);
		return i_pManager->_alloc(i_size);
	}

	void * alloc(HeapManager * i_pManager, size_t i_size, unsigned int i_alignment)
	{
		assert(i_pManager);

		return i_pManager->_alloc(i_size, i_alignment);
	}

	bool free(HeapManager * i_pManager, void * i_ptr)
	{
		assert(i_pManager);

		return i_pManager->_free(i_ptr);
	}

	void Collect(HeapManager * i_pManager)
	{
		assert(i_pManager);

		i_pManager->collect();
	}


	bool Contains(const HeapManager * i_pManager, void * i_ptr)
	{
		assert(i_pManager);

		return i_pManager->Contains(i_ptr);
	}

	bool IsAllocated(const HeapManager * i_pManager, void * i_ptr)
	{
		assert(i_pManager);

		return i_pManager->IsAllocated(i_ptr);
	}

	size_t GetLargestFreeBlock(const HeapManager * i_pManager)
	{
		assert(i_pManager);

		return i_pManager->getLargestFreeBlock();
	}

	size_t GetTotalFreeMemory(const HeapManager * i_pManager)
	{
		assert(i_pManager);

		return i_pManager->getTotalFreeMemory();
	}

	void ShowFreeBlocks(const HeapManager * i_pManager)
	{
		assert(i_pManager);

		i_pManager->ShowFreeBlocks();
	}

	void ShowOutstandingAllocations(const HeapManager * i_pManager)
	{
		assert(i_pManager);

#ifdef __TRACK_ALLOCATIONS
		i_pManager->ShowOutstandingAllocations();
#else
		printf("HeapManager compiled without __TRACK_ALLOCATIONS defined.\n");
#endif

	}



	HeapManager* HeapManager::create(void* heapMemory, size_t heapMemorySize) {

		HeapManager* heapManager = reinterpret_cast<HeapManager*>(heapMemory);
		heapManager->startOfHeap = reinterpret_cast<uint8_t*>(heapMemory) + sizeof(HeapManager);

		BlockDescriptor* temp = reinterpret_cast<BlockDescriptor*>(heapManager->startOfHeap);
		temp->sizeOfBlock = heapMemorySize - sizeof(HeapManager) - sizeof(BlockDescriptor);
		temp->startMemBlockAddr = reinterpret_cast<uint8_t*>(heapManager->startOfHeap) + sizeof(BlockDescriptor);
		temp->prevBlock = nullptr;
		temp->nextBlock = nullptr;
		heapManager->FreeBlocks = temp;
		heapManager->AllocatedBlocks = nullptr;

		heapManager->sizeOfHeap = heapMemorySize - sizeof(HeapManager);

		heapManagerInstance = heapManager;
		return heapManager;
	};


	void* HeapManager::_alloc(size_t const i_bytes, unsigned int i_alignment)
	{
		BlockDescriptor* temp = this->FreeBlocks;
		size_t alignmentOffset = 0;
		uint8_t* alignedPtr = nullptr;
		while (temp != nullptr)
		{
			alignmentOffset = checkAlignment(temp->startMemBlockAddr + temp->sizeOfBlock - i_bytes, i_alignment);
			alignedPtr = temp->startMemBlockAddr + temp->sizeOfBlock - i_bytes - alignmentOffset;
			if (alignedPtr - sizeof(BlockDescriptor) < temp->startMemBlockAddr)
			{
				temp = temp->nextBlock;
			}
			else
			{
				break;
			}
		}
		const size_t allocationSize = i_bytes + alignmentOffset;
		if (temp != nullptr)
		{
			BlockDescriptor * newAllocatedBlock = reinterpret_cast<BlockDescriptor*>(alignedPtr - sizeof(BlockDescriptor));
			newAllocatedBlock->startMemBlockAddr = alignedPtr;
			newAllocatedBlock->sizeOfBlock = allocationSize;
			newAllocatedBlock->nextBlock = this->AllocatedBlocks;
			newAllocatedBlock->prevBlock = nullptr;
			if (this->AllocatedBlocks != nullptr)
			{
				this->AllocatedBlocks->prevBlock = newAllocatedBlock;
			}
			this->AllocatedBlocks = newAllocatedBlock;
			temp->sizeOfBlock -= (allocationSize + sizeof(BlockDescriptor));

			return this->AllocatedBlocks->startMemBlockAddr;
		}
		else
		{
			printf("Allocation request cannot be satisfied\n");
			return nullptr;
		}

	}


	void HeapManager::insertionSort(BlockDescriptor* newBlock) {

		BlockDescriptor* tempFree = this->FreeBlocks;

		//put this newBlock in the free list using insertion sort
		BlockDescriptor * prev = nullptr;
		if (tempFree == nullptr)
		{
			newBlock->nextBlock = this->FreeBlocks;
			this->FreeBlocks = newBlock;
			this->FreeBlocks->prevBlock = nullptr;
		}
		else
		{
			while (tempFree != nullptr && newBlock > tempFree)
			{
				prev = tempFree;
				tempFree = tempFree->nextBlock;
			}
			if (tempFree != nullptr)
			{
				//if temp free is head
				if (tempFree == this->FreeBlocks)
				{
					newBlock->nextBlock = tempFree;
					newBlock->prevBlock = tempFree->prevBlock;
					tempFree->prevBlock = newBlock;
					this->FreeBlocks = newBlock;
				}
				//if temp free is not head
				else
				{
					newBlock->SetPrev(tempFree->prevBlock);
					newBlock->SetNext(tempFree);
				}
			}
			//newBlock will go at the end of the free list
			else
			{
				newBlock->SetPrev(prev);
				//prev->nextBlock = newBlock;
				newBlock->nextBlock = nullptr;
				//newBlock->prevBlock = prev;
			}
		}

	};

	bool HeapManager::_free(void* const i_ptr) {

		BlockDescriptor* tempAlloc = this->AllocatedBlocks;
		BlockDescriptor* tempFree = this->FreeBlocks;

		//finding a block in the allocated block list with the same startMemBlockAddr as i_ptr
		while (tempAlloc != nullptr) {
			if ((i_ptr >= tempAlloc->startMemBlockAddr) && (tempAlloc->startMemBlockAddr + tempAlloc->sizeOfBlock > i_ptr))
				break;
			tempAlloc = tempAlloc->nextBlock;

		}

		//the previous while loop found the correct tempAlloc block that should be freed
		if (tempAlloc != nullptr) {
			//if tempAlloc is the head of the alloc list
			if (tempAlloc == this->AllocatedBlocks)
			{
				this->AllocatedBlocks = this->AllocatedBlocks->nextBlock;
				if (this->AllocatedBlocks != nullptr)
					this->AllocatedBlocks->prevBlock = nullptr;
			}
			//if tempAlloc is not the head of the alloc list
			else {
				//if there exists a block before tempAlloc
				if (tempAlloc->prevBlock != nullptr)
				{
					//set tempAlloc's prev block's next block in allocated list to temp's next block
					tempAlloc->prevBlock->nextBlock = tempAlloc->nextBlock;

				}
				//if there exists a block after tempAlloc
				if (tempAlloc->nextBlock != nullptr)
				{
					//set temp's next block in allocated list to temp's prev block
					tempAlloc->nextBlock->prevBlock = tempAlloc->prevBlock;
				}
			}
			insertionSort(tempAlloc);

			return true;
		}
		else {
			printf("Invalid free request\n");
			return false;
		}
	};

	// attempt to merge abutting blocks.
	void HeapManager::collect() {

		BlockDescriptor* tempFree = this->FreeBlocks;

		while (tempFree != nullptr)
		{
			if (tempFree->nextBlock != nullptr) {


				if (tempFree->startMemBlockAddr + tempFree->sizeOfBlock == reinterpret_cast<uint8_t*>(tempFree->nextBlock))
				{
					tempFree->sizeOfBlock += tempFree->nextBlock->sizeOfBlock + sizeof(BlockDescriptor);
					//tempFree->alignedOffset = tempFree->nextBlock->alignedOffset;

					tempFree->nextBlock = tempFree->nextBlock->nextBlock;

					if (tempFree->nextBlock != nullptr) {
						tempFree->nextBlock->prevBlock = tempFree;
					}
				}
				else {
					tempFree = tempFree->nextBlock;
				}
			}
			else {
				tempFree = tempFree->nextBlock;
			}
		}

	};


	bool HeapManager::IsAllocated(void * const i_ptr) const {

		BlockDescriptor* temp = this->AllocatedBlocks;

		while (temp != nullptr) {
			if (temp->startMemBlockAddr == reinterpret_cast<uint8_t*>(i_ptr)) {
				return true;
			}
			temp = temp->nextBlock;
		}

		return false;
	};

	size_t HeapManager::getLargestFreeBlock() const {

		size_t largestFreeBlock = 0;

		if (this->FreeBlocks != nullptr)
		{
			BlockDescriptor* temp = this->FreeBlocks;

			while (temp != nullptr) {
				if (temp->sizeOfBlock > largestFreeBlock) {
					largestFreeBlock = temp->sizeOfBlock;
				}
				temp = temp->nextBlock;
			}
			return largestFreeBlock;
		}
		else
		{
			printf("No Free Blocks!\n");
			return NULL;
		}

	};

	size_t HeapManager::getTotalFreeMemory() const {

		if (this->FreeBlocks != nullptr)
		{
			size_t totalFreeMemory = 0;
			BlockDescriptor* temp = this->FreeBlocks;

			while (temp != nullptr) {
				totalFreeMemory += temp->sizeOfBlock;
				temp = temp->nextBlock;
			}
			return totalFreeMemory;
		}
		else
		{
			return 0;
		}
	}

	void HeapManager::SizeChecker() {
		size_t totalSize = 0;

		BlockDescriptor* temp = this->AllocatedBlocks;
		while (temp != nullptr)
		{
			printf("Alloc Size: %zu\n", temp->sizeOfBlock);
			totalSize += (temp->sizeOfBlock);
			temp = temp->nextBlock;
		}

		temp = this->FreeBlocks;
		while (temp != nullptr)
		{
			printf("Free Size: %zu\n", temp->sizeOfBlock);
			totalSize += (temp->sizeOfBlock);
			temp = temp->nextBlock;
		}

		printf("Total Size: %zu\n", totalSize);
	}

	void HeapManager::ShowFreeBlocks() const {
		BlockDescriptor* temp = this->FreeBlocks;
		printf("\nPrinting Free Blocks: \n");
		if (temp == nullptr)
			printf("No blocks available in Free List!\n");
		while (temp != nullptr) {
			printf("FreeBlock Size: %zu ", temp->sizeOfBlock);
			printf("FreeBlock Address: %p\n", (temp->startMemBlockAddr));
			temp = temp->nextBlock;
		}
	};

	void HeapManager::ShowOutstandingAllocations() const {
		BlockDescriptor* temp = this->AllocatedBlocks;

		printf("\nPrinting Allocated Blocks: \n");
		if (temp == nullptr)
			printf("No blocks available in Allocated List!\n");
		while (temp != nullptr) {
			printf("AllocatedBlock Size: %zu ", temp->sizeOfBlock);
			printf("AllocatedBlock Address: %p\n", (temp->startMemBlockAddr));
			temp = temp->nextBlock;
		}



	};

	void HeapManager::destroy() {
		if (this->AllocatedBlocks)
		{
			if (this->AllocatedBlocks->nextBlock != nullptr)
				this->AllocatedBlocks->nextBlock = nullptr;
			if (this->AllocatedBlocks->prevBlock != nullptr)
				this->AllocatedBlocks->prevBlock = nullptr;
			this->AllocatedBlocks = nullptr;
		}

		if (this->FreeBlocks)
		{
			if (this->FreeBlocks->nextBlock != nullptr)
				this->FreeBlocks->nextBlock = nullptr;
			if (this->FreeBlocks->prevBlock != nullptr)
				this->FreeBlocks->prevBlock = nullptr;
			this->FreeBlocks = nullptr;
		}
	};



}

////standard new
//void* operator new(size_t size)
//{
//	printf("\nstandard new\n");
//	assert(heapManagerInstance != nullptr);
//
//	return heapManagerInstance->_alloc(size);
//
//}
////array new
//void* operator new[](size_t size)
//{
//	printf("\narray new\n");
//	assert(heapManagerInstance != nullptr);
//
//	return heapManagerInstance->_alloc(size);
//};
////placement new
//void* operator new(size_t size, uint8_t* i_ptr)
//{
//	//printf("\nplacement new\n");
//	assert(heapManagerInstance != nullptr);
//	return i_ptr;
//};
////new with alignment
//void* operator new(size_t size, unsigned int i_alignment)
//{
//	printf("\nnew with alignment\n");
//	assert(heapManagerInstance != nullptr);
//
//	return heapManagerInstance->_alloc(size, i_alignment);
//};
//
////standard delete
//void operator delete(void* i_ptr)
//{
//	printf("\nStandard delete\n");
//	if (i_ptr)
//		heapManagerInstance->_free(i_ptr);
//}
//
//void operator delete[](void* i_ptr)
//{
//	printf("\nArray delete\n");
//	if (i_ptr)
//		heapManagerInstance->_free(i_ptr);
//}
//
//char* customStrDup(const char * src)
//{
//	char * dst = reinterpret_cast<char*>(heapManagerInstance->_alloc(strlen(src) + 1));
//	if (dst = nullptr) return nullptr;
//	strcpy_s(dst, sizeof(dst), src);
//	return dst;
//}
