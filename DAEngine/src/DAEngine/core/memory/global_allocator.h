#pragma once
#include "allocator.h"

namespace da::memory {

	class CGlobalAllocator : public IAllocator
	{
	public:
		CGlobalAllocator(uint8_t layer);
		~CGlobalAllocator();

		virtual void* allocate(size_t size) override;
		virtual void deallocate(void* ptr) override;
		virtual void* reallocate(void* ptr, size_t size) override;

	private:
		uint8_t m_layer = 0;

	};

}
