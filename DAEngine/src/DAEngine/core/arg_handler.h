#pragma once

#include "DAEngine/core/containers.h"

namespace da::core {

	class CArgHandler
	{
	public:
		static void initialize(int argc, const char** argv);
		static void shutdown();
		static size_t getArgc();
		static const TArray<CString, da::memory::CCoreAllocator>& getArgv();
		static bool contains(const char* arg);

	private:
		static int s_argc;
		static TArray<CString, da::memory::CCoreAllocator> s_args;

	};

}