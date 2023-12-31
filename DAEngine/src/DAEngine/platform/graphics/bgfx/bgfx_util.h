#pragma once

#include <bgfx/bgfx.h>

#define BGFXDESTROY(x) ASSERT(::bgfx::isValid(x)) ::bgfx::destroy(x); x = BGFX_INVALID_HANDLE;