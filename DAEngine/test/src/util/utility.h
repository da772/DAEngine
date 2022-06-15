#pragma once
#include <chrono>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>

namespace utility {

	inline std::string CurrentDateTime() {
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);

        std::ostringstream oss;
            oss << std::put_time(&tm, "%m-%d-%Y %H:%M:%S");
        return oss.str().c_str();
	}

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
		std::string s = "output/UnitTest_" + CurrentDateTime() + "_" + GetConfiguration() + ".log";
		std::replace(s.begin(), s.end(), ':', '-');
		std::erase(s, ' ');
		return s;
	}

	inline uint64_t GetTimeUS()
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}

}
