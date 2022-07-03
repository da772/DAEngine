#include "dapch.h"
#include "file.h"

namespace da::core
{
	CFile::CFile(const CString& path) : m_path(path)
	{
		FILE* f = fopen(m_path.cstr(), "rb");
		fseek(f, 0, SEEK_END);
		size_t m_size = ftell(f);
		fseek(f, 0, SEEK_SET);

		m_data = TArray<char, memory::CCoreAllocator>(m_size + 1);
		fread(m_data.data(), m_size, 1, f);
		fclose(f);
	}

	const CString& CFile::path() const
	{
		return m_path;
	}

	const char* CFile::data() const
	{
		return m_data.data();
	}

	const size_t CFile::size() const
	{
		return m_data.size();
	}

}