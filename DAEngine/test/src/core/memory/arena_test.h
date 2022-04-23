#pragma once

#include "test.h"

namespace da::core::memory { class CArena; }

class CArenaTest : public ITest
{
	TEST_CLASS(CArenaTest);

public:
	CArenaTest();
	void RunTests() override;

private:
	bool CreateTest();
	bool AllocateTest();
	bool FreeTest();


private:
	da::core::memory::CArena* m_arena;

};


