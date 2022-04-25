#include "list_test.h"

#include <iostream>

using namespace da::core::containers;

CListTest::CListTest()
{

}

void CListTest::RunTests()
{
	TEST_FUNC(PrimitiveTest);
	TEST_FUNC(ObjectTest);
}

bool CListTest::PrimitiveTest()
{
	TList<int> list1;

	for (int i = 0; i < 25; i++) {
		list1.Push(i);
	}
	TEST_ASSERT(list1.Size() == 25);

	int counter = 0;
	for (const int& i : list1) {
		TEST_ASSERT(i == counter++);
		list1[(size_t)counter-1] += 25;
		
	}

	counter = 0;
	for (int i = 0; i < 25; i++) {
		TEST_ASSERT(list1[i] == (i + 25) );
	}

	list1.Pop();
	TEST_ASSERT(list1.Size() == 24);

	counter = 0;
	for (int i = 0; i < 24; i++) {
		TEST_ASSERT(list1[i] == (i + 25));
		list1[i] = i;
	}

	list1.Insert(55, 12);
	int count = 0;
	for (int i = 0; i < 25; i++) {
		if (i == 12) {
			TEST_ASSERT(list1[i] == 55);
			continue;
		}
		else {
			TEST_ASSERT(list1[i] == (count));
		}
		count++;
	}

	TEnumerator<int> en = list1.Find(55);
	TEST_ASSERT(en != list1.end());
	list1.Remove(en);
	TEST_ASSERT(list1.Size() == 24);
	list1.Remove(24);
	TEST_ASSERT(list1.Size() == 23);
	for (int i = 0; i < 23; i++) {

		std::cout << i << " " << list1[i] << std::endl;
		TEST_ASSERT(list1[i] == (i));
	}

	return true;
}

class TestObject
{
public:
	inline TestObject(int x, int y) : x(x), y(y) {

	}

	inline TestObject() : x(-1), y(-1) {

	}

	inline bool operator==(const TestObject& rhs) const {
		return x == rhs.x && y == rhs.y;
	}

	int x, y;
};

bool CListTest::ObjectTest()
{


	return true;
}
