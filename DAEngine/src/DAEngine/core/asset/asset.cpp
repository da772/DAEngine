
#include "asset.h"

#include <iostream>
#include <filesystem>
#include <core/arg_handler.h>


namespace da
{
	CAsset::CAsset(const std::string& path, EAssetFlags flags) : m_path(getRelativePath(path)), m_flags(flags)
	{
		if (((uint8_t)m_flags & (uint8_t)EAssetFlags::Stream) == 0)
		{
			retrieveData();
			return;
		}

		retrieveInfo();
	}


	CAsset::CAsset(const std::string& name, size_t size) : m_path(name), m_size(size)
	{
		m_data.resize(size, '\0');
		m_hasData = true;
	}

	CAsset::CAsset()
	{
		m_hasData = true;
	}

	const size_t CAsset::size() const
	{
		return m_size;
	}

	const char* CAsset::data()
	{
		if (!m_hasData)
		{
			retrieveData();
		}

		return &m_data[0];
	}

	const std::string CAsset::path() const
	{
		return m_path;
	}

	bool CAsset::exists(const char* file)
	{
		return std::filesystem::exists(file);
	}

	void CAsset::retrieveData()
	{
		m_hasData = true;
		FILE* f = fopen(m_path.c_str(), "rb");

		LOG_ASSERT(f, ELogChannel::Core, "Failed to load file: %s", m_path.c_str());

		fseek(f, 0, SEEK_END);
		m_size = ftell(f);
		fseek(f, 0, SEEK_SET);

		m_data = std::vector<char>(m_size + 1);
		fread(m_data.data(), m_size, 1, f);
		fclose(f);
	}

	void CAsset::retrieveInfo()
	{
		FILE* f = fopen(m_path.c_str(), "rb");
		fseek(f, 0, SEEK_END);
		m_size = ftell(f);
		fseek(f, 0, SEEK_SET);
		fclose(f);
	}

	std::string CAsset::getRelativePath(const std::string& str)
	{
		return str;
		std::string root = da::core::CArgHandler::getRootPath();
		int index = root.find_last_of('/');
		if (index == -1) {
			index = root.find_last_of('\\');
			
		}
		if (index == -1) {
			return str;
		}

		return root.substr(0, index+1) + str;
	}

}