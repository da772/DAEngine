#include "arena_test.h"
#include "DAEngine/core/memory/arena/arena.h"

ArenaTest::ArenaTest() : m_arena(nullptr)
{

}

void ArenaTest::RunTests()
{
	TEST_FUNC(CreateTest);
	TEST_FUNC(FreeTest);
	TEST_FUNC(AllocateTest);
}

bool ArenaTest::CreateTest()
{
	m_arena = new da::core::memory::Arena(0x140);
	TEST_ASSERT(m_arena);

	return true;
}

bool ArenaTest::AllocateTest()
{
	for (int i = 0; i < 9; i++)
	{
		int* myInt = (int*)m_arena->Allocate(0x20);
		*myInt = i*1000;
		TEST_ASSERT(*myInt == i*1000);
	}

	return true;
}

bool ArenaTest::FreeTest()
{
	TEST_ASSERT(false);

	return true;
}
