#pragma once

#include "DAEngine/core/containers.h"

namespace da::core {

	class CArgHandler
	{
	public:
		static void initialize(int argc, const char** argv);
		static void shutdown();
		static size_t getArgc();
		static bool contains(const CHashString& arg);

	private:
		static int s_argc;
		static std::vector<CHashString> s_args;

	};

}