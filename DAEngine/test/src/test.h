#pragma once
#include "logger.h"
#ifndef DA_DEBUG
#define NDEBUG
#endif
#include <assert.h>
#include "utility.h"

#define TEST_CLASS(x) \
public: \
virtual const char* GetTestName() override { return #x; }; \
\

#define TEST_ASSERT(x) if (!x) { Logger::Log("[%s] [FAIL] [%s] Test Failed at %s : LINE %d", utility::CurrentDateTime().c_str(), __FUNCTION__, __FILE__, __LINE__); assert(x); return false; }

#define TEST_FUNC(x) if (x()) Logger::Log("[%s] [PASS] [%s] Runtime %d ms", utility::CurrentDateTime().c_str(), #x, 0);\

class Test
{
public:
	virtual void RunTests() = 0;
	virtual const char* GetTestName() = 0;


};