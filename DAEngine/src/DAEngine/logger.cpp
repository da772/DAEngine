#include "dapch.h"
#include "logger.h"

#include <regex>
#include <ostream>
#include <sstream>
#include <string>
#include <fstream>

#ifdef DA_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace da
{
	std::ofstream CLogger::s_outFile;

	void CLogger::initialize()
	{
		CLogger::s_outFile = std::ofstream(utility::GetLogFileName().cstr(), std::ofstream::out);
	}

	void CLogger::shutdown()
	{
		CLogger::s_outFile.close();
		s_outFile = {};
	}

	void CLogger::logInternal(const CString& message)
	{
		printf("%s", message.cstr());

		std::regex match_str = std::regex("\033[[][0-9][0-9][m]?");

		std::stringstream result;

		std::regex_replace(std::ostream_iterator<char>(result), message.begin().get(), (char*)&message[message.size()-1], match_str, "");

#ifdef DA_PLATFORM_WINDOWS
		OutputDebugStringA(result.str().c_str());
#endif
		
		CLogger::s_outFile << result.str();
		CLogger::s_outFile.flush();
	}

}