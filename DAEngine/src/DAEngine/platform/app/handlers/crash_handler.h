#pragma once

namespace da
{
	class CCrashHandler
	{
	public:
		static void registerCrashHandler();
		static void createDump();
	};
}