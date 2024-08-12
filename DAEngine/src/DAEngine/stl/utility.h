#pragma once

#include <string>

namespace da::utility {

	std::string CurrentDateTime();

	inline std::string GetConfiguration()
	{
#ifdef DA_DEBUG
		return "DEBUG";
#elif DA_RELEASE
		return "RELEASE";
#elif DA_FINAL
		return "FINAL";
#endif
	}

	inline std::string GetLogFileName()
	{
		std::string s = "dumps/ConsoleLog_" + GetConfiguration() + ".log";
		return s;
	}

	uint64_t GetTimeUS();

}
