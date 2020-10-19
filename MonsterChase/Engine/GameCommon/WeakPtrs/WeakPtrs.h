#pragma once
#include <iostream>
#include "../SmartPtrs/SmartPtrs.h"



template<class T>
class WeakPtrs
{
	
public:
	friend SmartPtrs<T>;
	
	WeakPtrs() :
	m_TheObject(nullptr),
	m_RefCount(nullptr)
	{
		
	}
	
	//standard constructor (creating a weak ptr from a smart ptr)
	WeakPtrs (const SmartPtrs<T> & i_other) :
	m_TheObject(i_other.m_TheObject),
	m_RefCount(i_other.m_RefCount)
	{
		if (m_RefCount)
			(*m_RefCount).WeakPtrs++;
	}
	
	//copy constructor
	WeakPtrs(const WeakPtrs & i_other) :
		m_TheObject(i_other.m_TheObject),
		m_RefCount(i_other.m_RefCount)
	{
		if (m_RefCount)
			(*m_RefCount).WeakPtrs++;
	}

	//assignment operator
	WeakPtrs& operator=(const WeakPtrs & i_other)
	{
		if (this != &i_other)
		{
			Release();

			m_TheObject = i_other.m_TheObject;
			m_RefCount = i_other.m_RefCount;

			if (m_RefCount)
				(*m_RefCount).WeakPtrs++;
		}
		return *this;
	}

	SmartPtrs<T> Acquire()
	{
		SmartPtrs<T> A(*this);
		return A;
	}

	void operator=(std::nullptr_t i_nullptr) const
	{
		Release();
	}

	~WeakPtrs()
	{
		Release();
	}

	operator bool() const
	{
		if (m_RefCount)
			return m_RefCount->SmartPtrs > 0;
		return false;
	}

private:
	void Release() const
	{
		if (m_RefCount)
		{
			if (--(*m_RefCount).WeakPtrs == 0)
			{
				//delete m_TheObject;
				delete m_RefCount;
			}
		}
		
	}
	T		 *			 m_TheObject;
	RefCount *           m_RefCount;
};
