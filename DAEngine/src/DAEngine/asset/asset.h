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
	public:
		CAsset(const std::string& path, EAssetFlags flags = EAssetFlags::None);

		const size_t size() const;
		const char* data();
		const std::string path() const;

	private:
		void retrieveData();
		void retrieveInfo();

	private:
		std::string m_path;
		std::vector<char> m_data;
		size_t m_size;
		bool m_hasData;
		EAssetFlags m_flags;
	};
}
