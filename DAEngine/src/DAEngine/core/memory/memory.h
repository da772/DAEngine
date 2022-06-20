#pragma once


void* operator new(size_t size);
void operator delete(void* ptr);
void* allocate(size_t size);
void deallocate(void* ptr);
void* reallocate(void* ptr, size_t size);

namespace da
{
	size_t getMemoryAllocated();
}