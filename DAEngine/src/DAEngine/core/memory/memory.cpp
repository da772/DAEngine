#include "dapch.h"
#include "memory.h"
#include <cstring>

static size_t s_memoryAllocated = 0;
static size_t s_layers[32] = { 0 };
static uint8_t s_layerQueue[16] = { 0 };
static uint8_t s_layerQueuePtr = 0;

static const char* s_memory_layer_name[(uint8_t)da::memory::EMemoryLayer::INVALID+1] = {
	"Global"
	,"Core"
	,"Graphics"
	,"Application"
	,"Debug"
	,"INVALID"
};

void* allocate(size_t size)
{
	size_t* p = (size_t*)malloc(size + sizeof(size_t));
	p[0] = size;
	s_memoryAllocated += size + sizeof(size_t);
	s_layers[s_layerQueue[s_layerQueuePtr]] += size + sizeof(size_t);
	assert(p);
	return (void*)(&p[1]);
}

void deallocate(void* ptr) {
	if (!ptr) return;
	size_t* p = (size_t*)ptr;
	size_t size = p[-1]; 
	s_memoryAllocated -= size + sizeof(size_t);
	s_layers[s_layerQueue[s_layerQueuePtr]] -= size + sizeof(size_t);
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
	s_layers[s_layerQueue[s_layerQueuePtr]] += size - oldSize;
	return (void*)&np[1];
}

void memorycopy(void* dst, void* src, size_t size)
{
	memcpy(dst, src, size);
}

void* operator new(size_t size)
{
	return allocate(size);
}

void operator delete(void* ptr)
{
	deallocate(ptr);
}


size_t da::memory::getMemoryAllocated()
{
	return s_memoryAllocated;
}

void da::memory::push_memory_layer(EMemoryLayer layer)
{
	assert(s_layerQueuePtr < 16);
	s_layerQueue[++s_layerQueuePtr] = (uint8_t)layer;
}

void da::memory::pop_memory_layer()
{
	if (s_layerQueuePtr == 0) return;
	s_layerQueue[s_layerQueuePtr--] = 0;

}

uint8_t da::memory::peek_memory_layer()
{
	return s_layerQueue[s_layerQueuePtr];
}

size_t* da::memory::get_memory_layers()
{
	return s_layers;
}

const char* da::memory::get_memory_layer_name(da::memory::EMemoryLayer layer)
{
	return s_memory_layer_name[(uint8_t)layer];
}
