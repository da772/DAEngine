#pragma once

void* operator new(size_t size);
void operator delete(void* ptr);
void* allocate(size_t size);
void deallocate(void* ptr);
void* reallocate(void* ptr, size_t size);
void memorycopy(void* dst, void* src, size_t size);

namespace da::memory
{
	enum class EMemoryLayer : uint8_t {
		Global,
		Core,
		Graphics,
		Application,
		ImGui,
		Debug,
		INVALID
	};

	size_t getMemoryAllocated();
	void push_memory_layer(EMemoryLayer layer);
	void pop_memory_layer();
	uint8_t peek_memory_layer();
	size_t* get_memory_layers();
	const char* get_memory_layer_name(da::memory::EMemoryLayer layer);
}