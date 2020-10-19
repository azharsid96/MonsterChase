#pragma once
#include <stdint.h>

namespace HeapManagerProxy {

	struct BlockDescriptor {
		uint8_t* startMemBlockAddr;
		size_t sizeOfBlock;
		BlockDescriptor* nextBlock;
		BlockDescriptor* prevBlock;

		void SetNext(BlockDescriptor* i_c)
		{
			nextBlock = i_c;
			i_c->prevBlock = this;
		}

		void SetPrev(BlockDescriptor* i_a)
		{
			prevBlock = i_a;
			i_a->nextBlock = this;
		}
	};

	class HeapManager {

	public:

		const static size_t s_MinimumToLeave = sizeof(BlockDescriptor);

		static HeapManager* create(void* heapMemory, size_t heapMemorySize);

		static inline size_t checkAlignment(const uint8_t * i_ptr, int i_alignment)
		{
			if (reinterpret_cast<size_t>(i_ptr) % i_alignment)
				return reinterpret_cast<size_t>(i_ptr) % i_alignment;
			return 0;
		}

		void* _alloc(size_t const i_bytes, unsigned int i_alignment = 4);

		bool _free(void* const i_ptr);

		void insertionSort(BlockDescriptor* newBlock);

		// attempt to merge abutting blocks.
		void collect();

		inline bool Contains(void * const i_ptr) const {

			return ((i_ptr >= this->startOfHeap) && (i_ptr <= (this->startOfHeap + this->sizeOfHeap)));
		};

		bool IsAllocated(void * const i_ptr) const;

		size_t getLargestFreeBlock() const;

		size_t getTotalFreeMemory() const;

		void SizeChecker();

		void ShowFreeBlocks() const;

		void ShowOutstandingAllocations() const;

		void destroy();


	private:
		//a head for free blocks linked list
		BlockDescriptor* FreeBlocks;
		//a head for allocated blocks linked list
		BlockDescriptor* AllocatedBlocks;
		//starting address of heap
		uint8_t* startOfHeap;
		//size of heap
		size_t sizeOfHeap;



	};

	HeapManager *	CreateHeapManager(void * i_pMemory, size_t i_sizeMemory, unsigned int i_numDescriptors);
	void			Destroy(HeapManager * i_pManager);

	void *			alloc(HeapManager * i_pManager, size_t i_size);
	void *			alloc(HeapManager * i_pManager, size_t i_size, unsigned int i_alignment);
	bool			free(HeapManager * i_pManager, void * i_ptr);

	void			Collect(HeapManager * i_pManager);

	bool			Contains(const HeapManager * i_pManager, void * i_ptr);
	bool			IsAllocated(const HeapManager * i_pManager, void * i_ptr);

	size_t			GetLargestFreeBlock(const HeapManager * i_pManager);
	size_t			GetTotalFreeMemory(const HeapManager * i_pManager);

	void			ShowFreeBlocks(const HeapManager * i_pManager);
	void			ShowOutstandingAllocations(const HeapManager * i_pManager);


}

//extern HeapManagerProxy::HeapManager *heapManagerInstance;
//
//void* operator new(size_t size);
//void* operator new[](size_t size);
//void* operator new(size_t size, uint8_t* i_ptr);
//void* operator new(size_t size, unsigned int i_alignment);
//
//void operator delete(void* i_ptr);
//void operator delete[](void* i_ptr);
//
//char* customStrDup(const char * src);
