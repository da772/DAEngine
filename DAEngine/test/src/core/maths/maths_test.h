#pragma once
#include "test.h"

class CMathsTest : public ITest
{
	TEST_CLASS(CMathsTest);

public:
	CMathsTest();

private:
	bool PrimeTest();

};


