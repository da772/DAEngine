#include "dapch.h"
#include "utility.h"
#include <chrono>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>

namespace da::utility
{
	CString CurrentDateTime()
	{
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);

        std::ostringstream oss;
            oss << std::put_time(&tm, "%m-%d-%Y %H:%M:%S");
		return oss.str().c_str();
	}

	uint64_t GetTimeUS()
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}

}
