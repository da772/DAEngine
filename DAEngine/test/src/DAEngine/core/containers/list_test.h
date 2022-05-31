#pragma once

#include "test/test.h"

class CListTest : public ITest
{
	TEST_CLASS(CListTest);

public:
	CListTest();

private:
	bool PrimitiveTest();
	bool ObjectTest();
	bool AllocTest();
	bool AllocTestVector();

};


