#include "arg_handler.h"
#include "logger.h"


namespace da::core
{
	std::vector<CHashString> CArgHandler::s_args;
	int CArgHandler::s_argc = 0;
	std::string CArgHandler::s_rootPath;


	void CArgHandler::initialize(int argc, const char** argv)
	{
		s_argc = argc;
		if (argc <= 0) return;
		s_args = std::vector<CHashString>(s_argc);
		s_rootPath = argv[0];
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

	bool CArgHandler::contains(const CHashString& arg) {
		return std::find(s_args.begin(), s_args.end(), arg) != s_args.end();
	}

	std::string CArgHandler::getRootPath()
	{
		return s_rootPath;
	}

}