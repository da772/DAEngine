#include "dapch.h"
#include "utility.h"
#include <chrono>
#include <iomanip>
#include <ctime>
#include <time.h>

namespace da::utility
{
	std::string CurrentDateTime()
	{
		char buffer[2048];
		time_t ltime = time(NULL);
#ifdef DA_UNIX
        snprintf(buffer, sizeof(buffer), "%s", asctime(localtime(&ltime)));
#else
        sprintf_s(buffer, sizeof(buffer), "%s", asctime(localtime(&ltime)));
#endif

		return std::string(buffer);
	}

	uint64_t GetTimeUS()
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	}

}
