#pragma once
#include <stdint.h>

// Extra defines go here (acts as config)
#define ASSERT(x) CCore::_assert(x, __FILE__, __LINE__)


namespace da
{
	class CCore {
	public:
		static void _assert(bool x, const char* file, long line);
	};
	
}