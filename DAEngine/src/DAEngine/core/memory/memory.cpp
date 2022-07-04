#include "dapch.h"
#include "memory.h"
#include <cstring>
#include "logger.h"

static size_t s_memoryAllocated = 0;
static size_t s_layers[32] = { 0 };
static uint8_t s_layerQueue[16] = { 0 };
static uint8_t s_layerQueuePtr = 0;

static const char* s_memory_layer_name[(uint8_t)da::memory::EMemoryLayer::INVALID + 1] = {
	"Global"
	,"Core"
	,"Graphics"
	,"Application"
	,"ImGui"
	,"Debug"
	,"STD"
	,"INVALID"
};

void* allocate(size_t size)
{
	da::memory::FMemoryData* p = (da::memory::FMemoryData*)malloc(size + sizeof(da::memory::FMemoryData));
	assert(p);
	p[0] = { s_layerQueue[s_layerQueuePtr], size };
	s_memoryAllocated += size + sizeof(da::memory::FMemoryData);
	s_layers[p->m_layer] += size + sizeof(da::memory::FMemoryData);
	return (void*)(&p[1]);
}

void deallocate(void* ptr) {
	if (!ptr) return;
	da::memory::FMemoryData* p = &((da::memory::FMemoryData*)ptr)[-1];
	size_t size = p->m_size;
	s_memoryAllocated -= size + sizeof(da::memory::FMemoryData);
	s_layers[p->m_layer] -= size + sizeof(da::memory::FMemoryData);
	free(p);
}

void* reallocate(void* ptr, size_t size)
{
	da::memory::FMemoryData* p = &((da::memory::FMemoryData*)ptr)[-1];
	size_t oldSize = p->m_size;
	da::memory::FMemoryData* np = (da::memory::FMemoryData*)realloc(p, size + sizeof(da::memory::FMemoryData));
	assert(np);
	np->m_size = size;
	s_memoryAllocated += size - oldSize;
	s_layers[np->m_layer] += size - oldSize;
	return (void*)&np[1];
}

#ifndef DA_UNIX
void* operator new(size_t size)
{
    return allocate(size);
}

void operator delete(void* ptr)
{
    deallocate(ptr);
}
#endif

void memorycopy(void* dst, void* src, size_t size)
{
	memcpy(dst, src, size);
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
