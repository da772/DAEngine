#include "test_manager.h"
#include "test.h"
#include "logger.h"
#include "utility.h"

#include "core/memory/arena_test.h"


TestManager::TestManager()
{
	RegisterTest<ArenaTest>();
}

TestManager::~TestManager()
{
	for (Test* t : m_tests)
	{
		delete t;
	}

	m_tests.clear();
}

void TestManager::RunTests()
{
	for (Test* t : m_tests)
	{
		Logger::Log("------------------------------------\n[%s]\n------------------------------------", t->GetTestName());
		uint64_t time = utility::GetTimeUS();
		t->RunTests();
		Logger::Log("\n[%s] Time Elapsed: %.3f ms\n", t->GetTestName(), (utility::GetTimeUS() - time) / 1000.f);
	}
}
