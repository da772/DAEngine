#include "test_manager.h"
#include "test.h"
#include "logger.h"

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
		Logger::Log("------------------------------------\n%s\n------------------------------------", t->GetTestName());
		t->RunTests();
	}
}
