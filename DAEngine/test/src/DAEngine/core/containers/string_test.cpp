#include "string_test.h"
#include <DAEngine/core/containers.h>

CStringTest::CStringTest()
{

}

bool CStringTest::RunTests()
{
	TEST_FUNC(StringTest);	
	TEST_END();
}

bool CStringTest::StringTest()
{
	CString str = "Test";

	TEST_ASSERT(strcmp(str.cstr(), "Test") == 0);
	TEST_ASSERT(str.size() == 4);

	str += "123";

	TEST_ASSERT(strcmp(str.cstr(), "Test123") == 0);

	TEST_ASSERT(str.size() == 7);

	str += CString("567");

	TEST_ASSERT(strcmp(str.cstr(), "Test123567") == 0);
	TEST_ASSERT(str.size() == 10);

	char c_str[] = "Test123567";

	size_t count = 0;

	for (const char& c : str)
	{
		TEST_ASSERT(c == c_str[count++]);
	}

	TEST_ASSERT(count - 1 == str.size());

	str.insert('a', 0);

	TEST_ASSERT(strcmp(str.cstr(), "aTest123567") == 0);

	str.insert("abc", 11);

	TEST_ASSERT(strcmp(str.cstr(), "aTest123567abc") == 0);

	str.insert(CString("abc"), 0);

	TEST_ASSERT(strcmp(str.cstr(), "abcaTest123567abc") == 0);

	str.insert("abc", 5);

	TEST_ASSERT(strcmp(str.cstr(), "abcaTabcest123567abc") == 0);

	str = "abc123";

	TEST_ASSERT(strcmp(str.cstr(), "abc123") == 0);

	str.clear();
	/* Disable SubStr for now
	CString subStr = str.substr(1, 3);

	TEST_ASSERT(strcmp(subStr.cstr(), "bc1") == 0);

	subStr = str.substr(3, 3);

	TEST_ASSERT(strcmp(subStr.cstr(), "123") == 0);
	TEST_ASSERT(subStr == "123");
	*/

	TArray<char> chArr(5);
	for (unsigned char i = 0; i < 5; i++) {
		chArr[i] = i + '0';
	}

	CString arrString(chArr);

	TEST_ASSERT(arrString == "01234")

	TEST_END();
}
