#pragma once
#include "daengine/core/memory/allocator.h"


namespace da::memory
{
	class CLocalAllocator : public IAllocator
	{
	public:
		CLocalAllocator();
		virtual ~CLocalAllocator();
		void* allocate(size_t size) override;
		void deallocate(void* ptr) override;
		void* reallocate(void* ptr, size_t size) override;
	protected:
		uint8_t* m_heap;
		size_t m_size;

	};
}