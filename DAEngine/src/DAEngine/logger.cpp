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
	CLogger* CLogger::s_logger = nullptr;

	void CLogger::initialize()
	{
		if (!s_logger)
			s_logger = new CLogger();
		s_logger->m_file = fopen(utility::GetLogFileName().cstr(), "w");
		//CLogger::s_outFile = std::ofstream(utility::GetLogFileName().cstr(), std::ofstream::out);
	}

	void CLogger::shutdown()
	{
		if (s_logger)
		{
			if (s_logger->m_file)
				fclose(s_logger->m_file);
			delete s_logger;
			s_logger = nullptr;
		}
		//CLogger::s_outFile.close();
	}

	void CLogger::logInternal(CString&& message)
	{
		/*
		std::regex match_str = std::regex("\033[[][0-9][0-9][m]?");
		std::stringstream result;
		std::regex_replace(std::ostream_iterator<char>(result), message.begin().get(), (char*)&message[message.size()-1], match_str, "");
		*/
		printf(message.cstr());
		
#ifdef DA_COLORED_OUT
		/* Remove colored artifacts
		for (uint8_t i = 0; i < (uint8_t)ELogColor::Invalid; i++) {
			message.remove(colorTypeMap[i]);
		}
		*/
#endif

		if (s_logger->m_file) {
			fputs(message.cstr(), s_logger->m_file);
			fflush(s_logger->m_file);
		}
		
#ifdef DA_PLATFORM_WINDOWS
		OutputDebugStringA(message.cstr());
#endif
		
		//CLogger::s_outFile << result.str();
		//CLogger::s_outFile.flush();
	}

}