#pragma once

#include <bx/allocator.h>

namespace da::platform {

	///
	class DefaultAllocator : public bx::AllocatorI
	{
	public:
		///
		DefaultAllocator();

		///
		virtual ~DefaultAllocator();

		///
		virtual void* realloc(
			void* _ptr
			, size_t _size
			, size_t _align
			, const char* _filePath
			, uint32_t _line
		) override;

		static inline DefaultAllocator* Get() { return &ms_defaultAllocator; }

	private:
		static DefaultAllocator ms_defaultAllocator;
	};
}