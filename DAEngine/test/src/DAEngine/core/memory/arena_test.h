#pragma once

#include "test/test.h"

class CMemoryArenaTest : public ITest
{
	TEST_CLASS(CMemoryArenaTest);

public:
	CMemoryArenaTest();

private:
	bool testAllocationDeallocation();
	bool testReset();
	bool speedTest();
	bool threadedTest();

};


