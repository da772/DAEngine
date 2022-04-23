#pragma once
#include <chrono>

namespace utility {

	inline std::string CurrentDateTime() {
		using namespace std::chrono;
		auto local = zoned_time{ current_zone(), system_clock::now() };
		std::stringstream ss;
		ss << local;
		return ss.str();
	}

	inline std::string GetLogFileName()
	{
		std::string s = "output/UnitTest_" + CurrentDateTime()+".log";
		std::replace(s.begin(), s.end(), ':', '-');
		return s;
	}

	inline uint64_t GetTimeUS()
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}

}