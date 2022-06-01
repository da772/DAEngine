#pragma once

#include "core/containers.h"

namespace da::utility {

	CString CurrentDateTime();

	inline CString GetConfiguration()
	{
#ifdef DA_DEBUG
		return "DEBUG";
#elif DA_RELEASE
		return "RELEASE";
#elif DA_FINAL
		return "FINAL";
#endif
	}

	inline CString GetLogFileName()
	{
		CString s = "dumps/ConsoleLog_" + CurrentDateTime() + "_" + GetConfiguration() + ".log";
		s.replace(':', '-');
		s.remove(' ');
		return s;
	}

	uint64_t GetTimeUS();

}