#include "dapch.h"
#include "local_allocator.h"
#include "memory.h"

namespace da::memory
{
	CLocalAllocator::CLocalAllocator() : m_heap(nullptr), m_size(0)
	{

	}

	CLocalAllocator::~CLocalAllocator()
	{

	}

	void* CLocalAllocator::allocate(size_t size)
	{
		return nullptr;
	}

	void CLocalAllocator::deallocate(void* ptr)
	{
		
	}

	void* CLocalAllocator::reallocate(void* ptr, size_t size)
	{
		return nullptr;
	}
}