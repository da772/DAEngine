#include "list_test.h"
#include <DAEngine/core/containers.h>

using namespace da;

CListTest::CListTest()
{

}

bool CListTest::RunTests()
{
	TEST_FUNC(PrimitiveTest);
	TEST_FUNC(ObjectTest);
	TEST_FUNC(AllocTest);
	TEST_FUNC(AllocTestVector);
	TEST_END();
}

bool CListTest::PrimitiveTest()
{
	List<int> list1;

	for (int i = 0; i < 25; i++) {
		list1.Push(i);
	}
	TEST_ASSERT(list1.Size() == 25);

	int counter = 0;
	for (const int& i : list1) {
		TEST_ASSERT(i == counter++);
		list1[(size_t)counter - 1] += 25;

	}

	counter = 0;
	for (int i = 0; i < 25; i++) {
		TEST_ASSERT(list1[i] == (i + 25));
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

	Enumerator<int> en = list1.Find(55);
	TEST_ASSERT(en != list1.end());
	list1.Remove(en);
	TEST_ASSERT(list1.Size() == 24);
	list1.Remove(24);
	TEST_ASSERT(list1.Size() == 23);
	for (int i = 0; i < 23; i++) {
		TEST_ASSERT(list1[i] == (i));
	}

	list1.Clear();

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

bool CListTest::AllocTest()
{
	List<int> ii;
	for (int i = 0; i < 1e6; i++) {
		ii.Push(i);
	}

	TEST_ASSERT(ii.Size() == 1e6);

	for (int i = 0; i < 1e6; i++) {
		ii.Pop();
	}

	TEST_ASSERT(ii.Size() == 0);

	return true;
}

bool CListTest::AllocTestVector()
{
	std::vector<int> ii;
	for (int i = 0; i < 1e6; i++) {
		ii.push_back(i);
	}

	TEST_ASSERT(ii.size() == 1e6);

	for (int i = 0; i < 1e6; i++) {
		ii.pop_back();
	}

	TEST_ASSERT(ii.size() == 0);

	return true;
}
