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

#if defined(DA_PLATFORM_WINDOWS) && defined(DA_REVIEW)
#define DA_COLORED_OUT
#endif

#if defined(DA_PLATFORM_MACOSX) || defined(DA_PLATFORM_LIUNX)
#define DA_UNIX
#endif

#if (defined(DA_PLATFORM_MACOSX) || defined(DA_PLATFORM_WINDOWS) || defined(DA_PLATFORM_LINUX)) && !defined(DA_TEST)
#define DA_WINDOW_GLFW
#define DA_GRAPHICS_VULKAN
#define DA_GRAPHICS_BGFX
#if DA_DEBUG || DA_RELEASE
#define BX_CONFIG_DEBUG 1
#else
#define BX_CONFIG_DEBUG 0
#endif
#endif

namespace da
{
	class CCore {
	public:
		static void _assert(bool x, const char* file, long line);
	};

}
