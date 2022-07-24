#include "dapch.h"
#include "asset.h"


namespace da
{
	CAsset::CAsset(const CString& path, EAssetFlags flags) : m_path(path), m_flags(flags)
	{
		if (((uint8_t)m_flags & (uint8_t)EAssetFlags::Stream) == 0)
		{
			retrieveData();
			return;
		}

		retrieveInfo();
	}

	const size_t CAsset::size() const
	{
		return m_size;
	}

	const char* const CAsset::data()
	{
		if (!m_hasData)
		{
			retrieveData();
		}

		return m_data.data();
	}

	const CString CAsset::path() const
	{
		return m_path;
	}

	void CAsset::retrieveData()
	{
		m_hasData = true;
		FILE* f = fopen(m_path.cstr(), "rb");
		fseek(f, 0, SEEK_END);
		m_size = ftell(f);
		fseek(f, 0, SEEK_SET);

		m_data = TArray<char, memory::CAssetAllocator>(m_size + 1);
		fread(m_data.data(), m_size, 1, f);
		fclose(f);
	}

	void CAsset::retrieveInfo()
	{
		FILE* f = fopen(m_path.cstr(), "rb");
		fseek(f, 0, SEEK_END);
		m_size = ftell(f);
		fseek(f, 0, SEEK_SET);
		fclose(f);
	}

}