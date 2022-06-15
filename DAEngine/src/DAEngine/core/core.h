#pragma once
#include <stdint.h>

#include <assert.h>

// Extra defines go here (acts as config)
#if !defined(DA_FINAL)
#define ASSERT(x) CCore::_assert(x, __FILE__, __LINE__); assert(x);
#else
#define ASSERT(...)
#endif

#if defined(DA_DEBUG) || defined(DA_RELEASE)
#define DA_REVIEW
#endif


namespace da
{
	class CCore {
	public:
		static void _assert(bool x, const char* file, long line);
	};
	
}
