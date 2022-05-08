#pragma once

#include "test.h"

class CVectorTest : public ITest
{
	TEST_CLASS(CVectorTest);

public:
	CVectorTest();

private:
	bool Vec4Tests();
	bool Vec3Tests();
	bool Vec2Tests();

};


