#include "dapch.h"
#include "arg_handler.h"
#include "logger.h"

namespace da::core
{
	const char** CArgHandler::s_argv = nullptr;
	int CArgHandler::s_argc = 0;


	void CArgHandler::initialize(int argc, const char** argv)
	{
		s_argc = argc;
		s_argv = argv;

		CString argList = "[%s] Initalized with args:";
		for (int i = 0; i < argc; i++) {
			argList += CString("\n") + argv[i];
		}

		CLogger::LogInfo(ELogChannel::Core, argList, NAMEOF(CArgHandler::initialize));
	}

	void CArgHandler::shutdown()
	{
		s_argv = nullptr;
		s_argc = 0;
	}
}