#pragma once
#include "enumerable.h"

template<typename T>
class TQueue : IEnumerable<T> {
public:
	

	TEnumerator<T> begin() override
	{
		return TEnumerator(m_ptr);
	}

	const TEnumerator<T> begin() const override
	{
		return TEnumerator(m_ptr);
	}

	const TEnumerator<T> end() const override
	{
		return TEnumerator(&m_ptr[m_size - 1]);
	}

	TEnumerator<T> end() override
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
