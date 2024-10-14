#pragma once

#include "daengine/core.h"

class CFileMapGenerator
{
public:
	static void initialize();
	static std::string get_filepath(CHashString hash);
	

private:
	static CFileMapGenerator s_fileMapGenerator;
	std::unordered_map<CHashString, std::string> m_fileMap;
};