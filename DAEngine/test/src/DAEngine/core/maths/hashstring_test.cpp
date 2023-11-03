#include "hashstring_test.h"
#include <DAEngine/core/containers.h>

CHashStringTest::CHashStringTest()
{

}

bool CHashStringTest::RunTests()
{
	TEST_BEGIN()
		TEST_FUNC(HashStringTests);
	TEST_END();
}

bool CHashStringTest::HashStringTests()
{

	TEST_ASSERT(CHashString("123") != CHashString("321"));
	TEST_ASSERT(CHashString("abc") == CHashString("abc"));
	TEST_ASSERT(CHashString("abc") != CHashString("abcabcabcabcabcabcbacbac"));
	TEST_ASSERT(CHashString("abcabcabcabcabcabcbacbacabcabcabcabcabcabcbacbacabcabcabcabcabcabcbacbac1") != CHashString("abcabcabcabcabcabcbacbacabcabcabcabcabcabcbacbacabcabcabcabcabcabcbacbac"));
	TEST_ASSERT(CHashString("abcabcabcabcabcabcbacbacabcabcabcabcabcabcbacbacabcabcabcabcabcabcbacbac1") == CHashString("abcabcabcabcabcabcbacbacabcabcabcabcabcabcbacbacabcabcabcabcabcabcbacbac1"));
    auto a = CHashString("abcabcabcabcabcabcbacbacabcabcabcabcabcabcbacbacabcabcabcabcabcabcbacbac1");
    
    auto b =CHashString("abcabcabcabcabcabcbacbacabcabcabcabcabcabcbacbacabcabcabcabcabcabcbacbac1");
	TEST_ASSERT(a== b);
	TEST_ASSERT(CHashString("abcabcabcabcabcabcbacbacabcabcabcabcabcabcbacbacabcabcabcabcabcabcbacbac1") == CHashString("abcabcabcabcabcabcbacbacabcabcabcabcabcabcbacbacabcabcabcabcabcabcbacbac1"));

	return true;
}
