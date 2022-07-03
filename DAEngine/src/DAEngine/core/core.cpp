#include "dapch.h"
#include "core.h"
#include <assert.h>

#include "logger.h"
#include <iostream>

void da::CCore::_assert(bool x, const char* file, long line)
{
	if (x) return;
	CLogger::LogError(ELogChannel::Core, "Assert: %s, LINE: %ld", file, line);
}