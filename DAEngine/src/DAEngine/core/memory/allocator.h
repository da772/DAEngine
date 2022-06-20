#pragma once

namespace da::memory {

	class IAllocator
	{
	public:
		inline IAllocator() {};
		virtual ~IAllocator() {};

		virtual void* allocate(size_t size) = 0;
		virtual void deallocate(void* ptr) = 0;
		virtual void* reallocate(void* ptr, size_t size) = 0;
	};


}