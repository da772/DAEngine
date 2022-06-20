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
	}

	void CArgHandler::shutdown()
	{
		s_argv = nullptr;
		s_argc = 0;
	}

	size_t CArgHandler::getArgc()
	{
		return s_argc;
	}

	const char** CArgHandler::getArgv()
	{
		return s_argv;
	}

}