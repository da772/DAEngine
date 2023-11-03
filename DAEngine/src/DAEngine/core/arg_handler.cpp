#include "dapch.h"
#include "arg_handler.h"
#include "logger.h"


namespace da::core
{
	std::vector<std::string> CArgHandler::s_args;
	int CArgHandler::s_argc = 0;

	void CArgHandler::initialize(int argc, const char** argv)
	{
		s_argc = argc;
		if (argc <= 0) return;
		s_args = std::vector<std::string>(s_argc);
		for (int i = 0; i < argc; i++) {
			s_args[i] = argv[i];
		}
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

	const std::vector<std::string>& CArgHandler::getArgv()
	{
		return s_args;
	}

	bool CArgHandler::contains(const char* arg) {
		return true;
		//return s_args.contains(std::string(arg));
	}

}