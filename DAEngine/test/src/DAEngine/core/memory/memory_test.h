#pragma once
#include "test/test.h"

class CMemoryTest : public ITest
{
	TEST_CLASS(CMemoryTest);

public:
	CMemoryTest();

private:
	bool CMemoryLayerTest();

};
