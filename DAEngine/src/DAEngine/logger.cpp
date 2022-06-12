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
	void* CLogger::s_outFile = nullptr;

	void CLogger::initialize()
	{
		if (!s_outFile)
			s_outFile = new std::ofstream(utility::GetLogFileName().cstr(), std::ofstream::out);

	}

	void CLogger::shutdown()
	{
		if (!s_outFile) {
			return;
		}

		(*(std::ofstream*)s_outFile).close();
		delete s_outFile;
		s_outFile = nullptr;
	}

	void CLogger::logInternal(const CString& message)
	{
		if (!s_outFile) return;
		printf("%s", message.cstr());

		std::regex match_str = std::regex("\033[[][0-9][0-9][m]?");

		std::stringstream result;

		std::regex_replace(std::ostream_iterator<char>(result), message.begin().get(), (char*)&message[message.size()-1], match_str, "");

#ifdef DA_PLATFORM_WINDOWS
		OutputDebugStringA(result.str().c_str());
#endif
		
		*(std::ofstream*)s_outFile << result.str();
		(*(std::ofstream*)s_outFile).flush();
	}

}