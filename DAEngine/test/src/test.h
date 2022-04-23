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

#define TEST_ASSERT(x) if (!x) { CLogger::Log("[%s] [FAIL] [%s] Test Failed at %s : LINE %d", utility::CurrentDateTime().c_str(), __FUNCTION__, __FILE__, __LINE__); assert(x); return false; }

#define TEST_FUNC(x) {const uint64_t strt = utility::GetTimeUS(); if (x()) CLogger::Log("[%s] [PASS] [%s] Runtime %.3f ms", utility::CurrentDateTime().c_str(), #x, (utility::GetTimeUS()-strt)/1000.f);}\

class ITest
{
public:
	virtual void RunTests() = 0;
	virtual const char* GetTestName() = 0;


};