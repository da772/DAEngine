#pragma once

#include "test.h"
#include <DAEngine/core/containers/list.h>

class CListTest : public ITest
{
	TEST_CLASS(CListTest);

public:
	CListTest();

private:
	bool PrimitiveTest();
	bool ObjectTest();

};

