#pragma once

#ifndef DA_UNIX
void* operator new(size_t size);
void operator delete(void* ptr);
#endif
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
		GPU,
		Application,
		Asset,
		ImGui,
		Debug,
		INVALID
	};

#pragma pack(push, 1)
	struct FMemoryData {
		uint8_t m_layer;
		size_t m_size;
	};
#pragma pack(pop)

	size_t getMemoryAllocated();
	void push_memory_layer(EMemoryLayer layer);
	void pop_memory_layer();
	uint8_t peek_memory_layer();
	size_t* get_memory_layers();
	const char* get_memory_layer_name(da::memory::EMemoryLayer layer);
}
