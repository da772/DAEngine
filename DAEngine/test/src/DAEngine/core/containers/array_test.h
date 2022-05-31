#pragma once
#include "test/test.h"

class CArrayTest : public ITest
{
	TEST_CLASS(CArrayTest);

public:
	CArrayTest();

private:
	bool PrimitiveTest();
	bool ObjectTest();

};


