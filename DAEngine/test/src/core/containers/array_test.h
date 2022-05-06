#pragma once

#include "test.h"
#include <DAEngine/core/containers/array.h>

class CArrayTest : public ITest
{
	TEST_CLASS(CArrayTest);

public:
	CArrayTest();

private:
	bool PrimitiveTest();
	bool ObjectTest();

};


