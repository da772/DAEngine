#include "test_manager.h"
#include "test/test.h"
#include "util/logger.h"
#include "util/utility.h"

#include "DAEngine/core/containers/array_test.h"
#include "DAEngine/core/containers/list_test.h"
#include "DAEngine/core/containers/string_test.h"
#include "DAEngine/core/maths/hashstring_test.h"
#include "DAEngine/core/maths/maths_test.h"
#include "DAEngine/core/maths/vector_test.h"
#include "DAEngine/core/guid_test.h"
#include "DAEngine/core/memory/memory_test.h"
#include "DAEngine/app_test.h"


CTestManager::CTestManager()
{
	RegisterTest<CMemoryTest>();
	RegisterTest<CArrayTest>();
	RegisterTest<CListTest>();
	RegisterTest<CStringTest>();
	RegisterTest<CMathsTest>();
	RegisterTest<CVectorTest>();
	RegisterTest<CHashStringTest>();
	RegisterTest<CGuidTest>();
	RegisterTest<CAppTest>();
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
	std::vector<ITest*> failedTests;
	const auto start_time = utility::GetTimeUS();
	CLogger::Log("----------------------------------------------\n[Tests Begin]\n----------------------------------------------", (utility::GetTimeUS() - start_time) / 1000.f);
	for (ITest* t : m_tests)
	{
		CLogger::Log("------------------------------------\n[%s]\n------------------------------------", t->GetTestName());
		uint64_t time = utility::GetTimeUS();
		if (!t->RunTests())
			failedTests.push_back(t);
		CLogger::Log("%s\n[%s] Time Elapsed: %.3f ms\n%s", color::FG_CYAN, t->GetTestName(), (utility::GetTimeUS() - time) / 1000.f, color::FG_DEFAULT);
	}

	if (failedTests.size()) 
		CLogger::Log("%s------------------------------------\n[Tests Failed]\n------------------------------------%s", color::FG_RED, color::FG_DEFAULT);
	else
		CLogger::Log("%s------------------------------------\n[Tests Passed]\n------------------------------------%s", color::FG_GREEN, color::FG_DEFAULT);

	for (ITest* t : failedTests)
	{
		CLogger::Log("[%s]\n%s\n", t->GetTestName(), t->GetError());
	}

	CLogger::Log("%s----------------------------------------------\n[Tests Complete -  Time Elapsed: %.3f ms]\n----------------------------------------------%s", 
		color::FG_CYAN, (utility::GetTimeUS() - start_time) / 1000.f, color::FG_DEFAULT);
}
