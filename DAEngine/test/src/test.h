#pragma once
#include "logger.h"
#include <DAEngine/Core/core.h>
#ifndef DA_DEBUG
#define NDEBUG
#endif
#include <assert.h>
#include "utility.h"

#define TEST_CLASS(x) \
public: \
virtual const char* GetTestName() override { return #x; }; \
virtual bool RunTests() override; \
\

#define TEST_ASSERT(x) if (!(x)) { m_failReason = CLogger::Format("%s[%s] [FAIL] [%s] Test Failed at %s : LINE %d : %s", color::FG_RED, utility::CurrentDateTime().c_str(), __FUNCTION__, __FILE__, __LINE__, #x); CLogger::Log("%s", m_failReason.c_str()); return false; }
#define TEST_FUNC(x) {const uint64_t strt = utility::GetTimeUS(); if (x()) CLogger::Log("%s[%s] [PASS] [%s] Runtime %.3f ms", color::FG_GREEN, utility::CurrentDateTime().c_str(), #x, (utility::GetTimeUS()-strt)/1000.f); else return false; }
#define TEST_END() return true;

class ITest
{
public:
	virtual bool RunTests() = 0;
	virtual const char* GetTestName() = 0;
	inline const char* GetError() { return m_failReason.c_str(); };

protected:
	std::string m_failReason;


};