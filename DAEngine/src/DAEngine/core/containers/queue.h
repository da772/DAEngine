#pragma once
#include "enumerable.h"

template<typename T>
class TQueue : IEnumerable<T> {
public:
	

	TEnumerator<T> begin() const override
	{
		return TEnumerator(m_ptr);
	}


	TEnumerator<T> end() const override
	{
		return TEnumerator(&m_ptr[m_size - 1]);
	}


	size_t size() const override
	{
		return m_size;
	}

private:
	size_t m_size;
	size_t m_heapSize;
	T* m_ptr;

};
