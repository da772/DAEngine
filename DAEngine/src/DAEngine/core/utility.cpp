#include "dapch.h"
#include "utility.h"
#include <chrono>

namespace da::utility
{
	CString CurrentDateTime()
	{
		using namespace std::chrono;
		auto local = zoned_time{ current_zone(), system_clock::now() };
		std::stringstream ss;
		ss << local;
		return ss.str().c_str();
	}

	uint64_t GetTimeUS()
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}

}