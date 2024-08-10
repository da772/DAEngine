#pragma once
#include <assert.h>

#define DA_VERSION "0.01"

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
#define BX_CONFIG_DEBUG 0
#else
#define BX_CONFIG_DEBUG 0
#endif
#endif

#ifdef DA_PROFILE
#include "optick.h"
#define PROFILE_FRAME(x) OPTICK_FRAME(x);
#define PROFILE(...) OPTICK_EVENT(##__VA_ARGS__)
#define PROFILE_NAME(name) OPTICK_EVENT(__FUNCTION__ "::" name)
#define PROFILE_THREAD(x) OPTICK_THREAD(x)
#define PROFILE_TAG(NAME, ...) OPTICK_TAG(NAME, ##__VA_ARGS__)
#else
#define PROFILE_FRAME(x) 
#define PROFILE(...) 
#define PROFILE_NAME(name)
#define PROFILE_THREAD(x) 
#define PROFILE_TAG(...) 
#endif