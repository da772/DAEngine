#include "maths_test.h"
#include <DAEngine/core/maths/maths.h>

using namespace da::core;

CMathsTest::CMathsTest()
{

}

bool CMathsTest::RunTests()
{
	TEST_FUNC(PrimeTest);

	TEST_END();
}

bool CMathsTest::PrimeTest()
{
	int n = 1;
	int result = maths::NextPrime(n);
	TEST_ASSERT(result == 2);

	n = 1201;
	result = maths::NextPrime(n);
	TEST_ASSERT(result == 1213);


	TEST_END();
}