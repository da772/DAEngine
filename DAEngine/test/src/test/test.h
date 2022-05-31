#pragma once
#include "util/logger.h"
#include <DAEngine/Core/core.h>
#ifndef DA_DEBUG
#define NDEBUG
#endif
#include <assert.h>
#include "util/utility.h"

#define TEST_CLASS(x) \
public: \
virtual const char* GetTestName() override { return #x; }; \
virtual bool RunTests() override; \
\

#define TEST_ASSERT(x) if (!(x)) { m_failReason = CLogger::Format("%s[%s] [FAIL] [%s] Test Failed at %s : LINE %d : %s%s", color::FG_RED, utility::CurrentDateTime().c_str(), __FUNCTION__, __FILE__, __LINE__, #x, color::FG_DEFAULT); CLogger::Log("%s", m_failReason.c_str()); return false; }
#define TEST_FUNC(x) {const uint64_t strt = utility::GetTimeUS(); if (x()) CLogger::Log("%s[%s] [PASS] [%s] Runtime %.3f ms%s", color::FG_GREEN, utility::CurrentDateTime().c_str(), #x, (utility::GetTimeUS()-strt)/1000.f, color::FG_DEFAULT); else return false; }
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