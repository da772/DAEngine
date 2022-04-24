#pragma once

#include "test.h"
#include <DAEngine/core/containers/array.h>

class CArrayTest : public ITest
{
	TEST_CLASS(CArrayTest);

public:
	CArrayTest();
	void RunTests() override;

private:
	bool PrimitiveTest();
	bool ObjectTest();


private:
	da::core::containers::TArray<int> m_array;

};


