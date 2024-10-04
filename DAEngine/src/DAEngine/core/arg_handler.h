#pragma once

#include "dastd.h"

namespace da {

	class CArgHandler
	{
	public:
		static void initialize(int argc, const char** argv);
		static void shutdown();
		static size_t getArgc();
		static std::string getRootPath();
		static bool contains(const CHashString& arg);

	private:
		static int s_argc;
		static std::vector<CHashString> s_args;
		static std::string s_rootPath;

	};

}