#pragma once
#include "DAEngine/core/containers.h"
#include <stdio.h>

namespace da::core
{
	class CFile
	{
	public:
		CFile(const CString& path);

		const CString& path() const;
		const char* data() const;
		const size_t size() const;

	private:
		CBasicString<memory::CCoreAllocator> m_path;
		TArray<char,memory::CCoreAllocator> m_data;
	};
}
