#pragma once

#include "test.h"

namespace da { namespace core { namespace memory { class Arena; } } }

class ArenaTest : public Test
{
	TEST_CLASS(ArenaTest);

public:
	ArenaTest();
	void RunTests() override;

private:
	bool CreateTest();
	bool AllocateTest();
	bool FreeTest();


private:
	da::core::memory::Arena* m_arena;

};


