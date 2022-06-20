#pragma once
#include "allocator.h"

namespace da::memory {
	class CGlobalAllocator : public IAllocator
	{
	public:
		CGlobalAllocator(uint8_t layer = (uint8_t)EMemoryLayer::Global);
		virtual ~CGlobalAllocator();

		virtual void* allocate(size_t size) override;
		virtual void deallocate(void* ptr) override;
		virtual void* reallocate(void* ptr, size_t size) override;

	protected:
		uint8_t m_layer = 0;
	};

	class CCoreAllocator : public CGlobalAllocator
	{
	public:
		inline CCoreAllocator() : CGlobalAllocator((uint8_t)EMemoryLayer::Core) {};
	};

	class CGraphicsAllocator : public CGlobalAllocator
	{
	public:
		inline CGraphicsAllocator() : CGlobalAllocator((uint8_t)EMemoryLayer::Graphics) {};
	};

	class CApplicationAllocator : public CGlobalAllocator
	{
	public:
		inline CApplicationAllocator() : CGlobalAllocator((uint8_t)EMemoryLayer::Application) {};
	};

	class CImGuiAllocator : public CGlobalAllocator
	{
	public:
		inline CImGuiAllocator() : CGlobalAllocator((uint8_t)EMemoryLayer::ImGui) {};
	};

	class CDebugAllocator : public CGlobalAllocator
	{
	public:
		inline CDebugAllocator() : CGlobalAllocator((uint8_t)EMemoryLayer::Debug) {};
	};
}
