#include "dapch.h"
#include "utility.h"
#include <chrono>
#include <iomanip>
#include <ctime>
#include <time.h>

namespace da::utility
{
	CString CurrentDateTime()
	{
		char buffer[2048];
		time_t ltime = time(NULL);
		sprintf_s(buffer, sizeof(buffer), "%s", asctime(localtime(&ltime)));

		return CString(buffer);
	}

	uint64_t GetTimeUS()
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}

}
