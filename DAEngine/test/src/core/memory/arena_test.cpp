#include "arena_test.h"
#include "DAEngine/core/memory/arena/arena.h"

CArenaTest::CArenaTest() : m_arena(nullptr)
{

}

void CArenaTest::RunTests()
{
	TEST_FUNC(CreateTest);
	TEST_FUNC(AllocateTest);
	TEST_FUNC(FreeTest);
}

bool CArenaTest::CreateTest()
{
	TEST_ASSERT(!m_arena)
	m_arena = new da::core::memory::CArena(0x140);
	TEST_ASSERT(m_arena);

	return true;
}

bool CArenaTest::AllocateTest()
{
	TEST_ASSERT(m_arena);
	for (int i = 0; i < 9; i++)
	{
		int* myInt = (int*)m_arena->Allocate(0x20);
		*myInt = i*1000;
		TEST_ASSERT(*myInt == i*1000);
	}

	return true;
}

bool CArenaTest::FreeTest()
{
	TEST_ASSERT(m_arena);
	m_arena->Free();
	m_arena = nullptr;
	TEST_ASSERT(!m_arena);

	return true;
}
