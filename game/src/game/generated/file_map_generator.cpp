#include "file_map_generator.h"
#include "dacommon.h"

#include <filesystem>

void CFileMapGenerator::initialize()
{
	PROFILE();
	const std::filesystem::path p = std::filesystem::path(std::filesystem::current_path().string() + "\\assets");

	for (const auto& dirEntry : std::filesystem::recursive_directory_iterator(p))
	{
		if (dirEntry.is_directory()) {
			continue;
		}

		size_t it = dirEntry.path().string().find("source");
		if (it != std::string::npos)
		{
			continue;
		}

		std::string dir = dirEntry.path().string();
		size_t asIt = dir.find("assets");
	
		dir.erase(0, asIt);

		CHashString hash = HASHSTR(dir.c_str());

		if (hash.hash() == 2688253374725150482)
		{
			ASSERT(true);
		}

		s_fileMapGenerator.m_fileMap[hash] = dirEntry.path().string();
	}
}

std::string CFileMapGenerator::get_filepath(CHashString hash)
{
	return s_fileMapGenerator.m_fileMap[hash];
}

CFileMapGenerator CFileMapGenerator::s_fileMapGenerator;
