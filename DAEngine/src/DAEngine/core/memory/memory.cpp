#include "dapch.h"
#include "memory.h"
#include <cstring>

static size_t s_memoryAllocated = 0;

void* allocate(size_t size)
{
	size_t* p = (size_t*)malloc(size + sizeof(size_t));
	p[0] = size;
	s_memoryAllocated += size + sizeof(size_t);
	assert(p);
	return (void*)(&p[1]);
}

void deallocate(void* ptr) {
	size_t* p = (size_t*)ptr;
	size_t size = p[-1]; 
	s_memoryAllocated -= size + sizeof(size_t);
	void* p2 = (void*)(&p[-1]);
	free(p2);                   
}

void* reallocate(void* ptr, size_t size)
{
	size_t* p = (size_t*)ptr;
	size_t oldSize = p[-1];
	size_t* np = (size_t*)realloc(&p[-1], size + sizeof(size_t));
	assert(np);
	np[0] = size;
	s_memoryAllocated += size - oldSize;
	return (void*)&np[1];
}

void* operator new(size_t size)
{
	return allocate(size);
}

void operator delete(void* ptr)
{
	deallocate(ptr);
}


size_t da::getMemoryAllocated()
{
	return s_memoryAllocated;
}
