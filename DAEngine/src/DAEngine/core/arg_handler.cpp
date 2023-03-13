#include "dapch.h"
#include "arg_handler.h"
#include "logger.h"


namespace da::core
{
	TArray<CString, da::memory::CCoreAllocator> CArgHandler::s_args;
	int CArgHandler::s_argc = 0;

	void CArgHandler::initialize(int argc, const char** argv)
	{
		s_argc = argc;
		s_args = TArray<CString, da::memory::CCoreAllocator>(s_argc);
		for (int i = 0; i < argc; i++) {
			s_args[i] = argv[i];
		}
		return;
	}

	void CArgHandler::shutdown()
	{
		s_args.clear();
		s_argc = 0;
	}

	size_t CArgHandler::getArgc()
	{
		return s_argc;
	}

	const TArray<CString, da::memory::CCoreAllocator>& CArgHandler::getArgv()
	{
		return s_args;
	}

	bool CArgHandler::contains(const char* arg) {
		return s_args.contains(arg);
	}

}