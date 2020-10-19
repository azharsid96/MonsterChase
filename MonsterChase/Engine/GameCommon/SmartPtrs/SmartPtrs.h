#pragma once
#include <iostream>


template<class T>
class WeakPtrs;

struct RefCount
{
	int     SmartPtrs;
	int     WeakPtrs;
	RefCount(int i_SmartPtrs, int i_WeakPtrs) :
		SmartPtrs(i_SmartPtrs),
		WeakPtrs(i_WeakPtrs)
		{
		}
};

template<class T>
class SmartPtrs
{
	friend WeakPtrs<T>;
public:
	//standard constructor
	
	explicit SmartPtrs(T * i_TheObject) :
		m_TheObject(i_TheObject),
		m_RefCount(i_TheObject ? new RefCount(1, 0) : nullptr)
		{
			
		}
	//making a smart pointer from a weak pointer
	inline explicit SmartPtrs(WeakPtrs<T>& i_WeakPtrs):
	m_TheObject(i_WeakPtrs.m_RefCount->SmartPtrs != 0 ? i_WeakPtrs.m_TheObject : nullptr),
	m_RefCount(i_WeakPtrs.m_RefCount)
	{
		(*m_RefCount).SmartPtrs++;
	}

	//copy constructor
	SmartPtrs(const SmartPtrs & i_other) :
		m_TheObject(i_other.m_TheObject),
		m_RefCount(i_other.m_RefCount)
	{
		(*m_RefCount).SmartPtrs++;
	}

	//assignment operator
	inline SmartPtrs& operator=(SmartPtrs & i_other)
	{
		if (this != &i_other)
		{
			Release();

			m_TheObject = i_other.m_TheObject;
			m_RefCount = i_other.m_RefCount;

			(*m_RefCount).SmartPtrs++;
		}
	}
	//assigning smart pointer to null
	void operator=(std::nullptr_t i_nullptr) const
	{
		Release();
	}

	//Dereferencing operator - First way
	T * operator->()
	{
		return m_TheObject;
	}

	//Dereferencing operator - Second way
	T & operator*()
	{
		return (*m_TheObject);
	}
	//is smart pointer valid or not
	operator bool()
	{
		return m_TheObject != nullptr;
	}

	~SmartPtrs()
	{
		Release();
	}
	
private:
	void Release() const
	{
		if (m_RefCount)
		{
			if (--(*m_RefCount).SmartPtrs == 0)
			{
				delete m_TheObject;
				delete m_RefCount;
			}
		}
	}
	T		 *			 m_TheObject;
	RefCount *           m_RefCount;
};