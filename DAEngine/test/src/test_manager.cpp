#include "test_manager.h"
#include "test.h"
#include "logger.h"
#include "utility.h"

#include "core/containers/array_test.h"
#include "core/memory/arena_test.h"


CTestManager::CTestManager()
{
	RegisterTest<CArrayTest>();
	RegisterTest<CArenaTest>();
}

CTestManager::~CTestManager()
{
	for (ITest* t : m_tests)
	{
		delete t;
	}

	m_tests.clear();
}

void CTestManager::RunTests()
{
	const auto start_time = utility::GetTimeUS();
	CLogger::Log("----------------------------------------------\n[Tests Begin]\n----------------------------------------------", (utility::GetTimeUS() - start_time) / 1000.f);
	for (ITest* t : m_tests)
	{
		CLogger::Log("------------------------------------\n[%s]\n------------------------------------", t->GetTestName());
		uint64_t time = utility::GetTimeUS();
		t->RunTests();
		CLogger::Log("\n[%s] Time Elapsed: %.3f ms\n", t->GetTestName(), (utility::GetTimeUS() - time) / 1000.f);
	}
	CLogger::Log("----------------------------------------------\n[Tests Complete -  Time Elapsed: %.3f ms]\n----------------------------------------------", (utility::GetTimeUS() - start_time) / 1000.f);
}
