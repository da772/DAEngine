#include "dapch.h"
#include "global_allocator.h"
#include "memory.h"

namespace da::memory
{
	CGlobalAllocator::CGlobalAllocator(uint8_t layer) : m_layer(layer)
	{

	}

	CGlobalAllocator::~CGlobalAllocator()
	{

	}

	void* CGlobalAllocator::allocate(size_t size)
	{
		da::memory::push_memory_layer((da::memory::EMemoryLayer)m_layer);
		void* ptr = ::allocate(size);;
		da::memory::pop_memory_layer();
		return ptr;
	}

	void CGlobalAllocator::deallocate(void* ptr)
	{
		::deallocate(ptr);
		return;
	}

	void* CGlobalAllocator::reallocate(void* ptr, size_t size)
	{
		void* p = ::reallocate(ptr, size);
		return p;
	}

}