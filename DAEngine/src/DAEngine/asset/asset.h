#pragma once
#include "DAEngine/core/containers.h"


namespace da
{
	enum class EAssetFlags : uint8_t {
		None = 0x00,
		Stream = 0x01
	};

	class CAsset
	{
	public :
		CAsset(const CString& path, EAssetFlags flags = EAssetFlags::None);

		const size_t size() const;
		const char* const data();
		const CString path() const;

	private:
		void retrieveData();
		void retrieveInfo();

	private:
		CBasicString<da::memory::CAssetAllocator> m_path;
		TArray<char, da::memory::CAssetAllocator> m_data;
		size_t m_size;
		bool m_hasData;
		EAssetFlags m_flags;
	};
}
