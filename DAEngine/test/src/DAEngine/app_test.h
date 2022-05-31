#pragma once
#include "test/test.h"

class CAppTest : public ITest
{
	TEST_CLASS(CAppTest);

public:
	CAppTest();

private:
	bool AppTests();

};


