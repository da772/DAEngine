#include "list_test.h"
#include <DAEngine/core/containers.h>

CListTest::CListTest()
{

}

bool CListTest::RunTests()
{
	TEST_BEGIN()
		TEST_FUNC(PrimitiveTest);
		TEST_FUNC(ObjectTest);
		TEST_FUNC(AllocTest);
		TEST_FUNC(AllocTestVector);
	TEST_END();
}

bool CListTest::PrimitiveTest()
{
	TList<int> list1;

	for (int i = 0; i < 25; i++) {
		list1.push(i);
	}
	TEST_ASSERT(list1.size() == 25);

	int counter = 0;
	for (const int& i : list1) {
		TEST_ASSERT(i == counter++);
		list1[(size_t)counter - 1] += 25;

	}

	counter = 0;
	for (int i = 0; i < 25; i++) {
		TEST_ASSERT(list1[i] == (i + 25));
	}

	list1.pop();
	TEST_ASSERT(list1.size() == 24);
	
	counter = 0;
	for (int i = 0; i < 24; i++) {
		TEST_ASSERT(list1[i] == (i + 25));
		list1[i] = i;
	}

	list1.insert(55, 12);
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
	
	TList<int> list2 = TList<int>(list1);
	
	TEST_ASSERT(list1 == list2);
	
	TList<int> list3 = TList<int>(std::move(list2));
	
	TEnumerator<int> en = list1.find(55);

	TEnumerator<int> en1 = list1.find([](const int& i) {
		if (i == 55) return true;
		return false;
	});
	
	TEST_ASSERT(en != list1.end());
	list1.remove(en);
	TEST_ASSERT(list1.size() == 24);
	list1.remove(24);
	TEST_ASSERT(list1.size() == 23);
	for (int i = 0; i < 23; i++) {
		TEST_ASSERT(list1[i] == (i));
	}

	list1.clear();

	TList<uint64_t> tstList;
	tstList.resize(2);

	tstList.resize(4);

	return true;
}

class TestObject
{
public:
	inline TestObject(int x, int y) : x(x), y(y) {

	}

	inline bool operator==(const TestObject& rhs) const {
		return x == rhs.x && y == rhs.y;
	}

	int x, y;
};

bool CListTest::ObjectTest()
{
	
	TList<TestObject> list;

	list.push(TestObject(0, 1));
	list.push(TestObject(2, 1));
	list.push(TestObject(3, 4));
	list.push(TestObject(3, 6));
	
	TEST_ASSERT(list.size() == 4);

	TEnumerator<TestObject> enumerator = list.find([](const TestObject& i) { return i.x == 0; });

	TEST_ASSERT(enumerator != list.end());

	enumerator = list.find(TestObject(0, 1));

	TEST_ASSERT(enumerator != list.end());

	enumerator = list.find(TestObject(0, 12));

	TEST_ASSERT(enumerator == list.end());

	for (TestObject& obj : list) {
		obj.x = 15;
	}

	for (const TestObject& obj : list) {
		TEST_ASSERT(obj.x == 15);
	}

	
	return true;
}

bool CListTest::AllocTest()
{
	{
		TList<int> ii;
		for (int i = 0; i < 1e6; i++) {
			ii.push(i);
		}

		TEST_ASSERT(ii.size() == 1e6);

		for (int i = 0; i < 1e6; i++) {
			TEST_ASSERT(ii[i] == i);
		}

		for (int i = 0; i < 1e6; i++) {
			ii.pop();
		}

		TEST_ASSERT(ii.size() == 0);

	}

	{
		TList<TestObject> ii;
		for (int i = 0; i < 1e6; i++) {
			ii.push(TestObject(i, i));
		}

		TEST_ASSERT(ii.size() == 1e6);

		for (int i = 0; i < 1e6; i++) {
			TEST_ASSERT( ii[i] == TestObject(i, i));
		}

		for (int i = 0; i < 1e6; i++) {
			ii.pop();
		}

		TEST_ASSERT(ii.size() == 0);
	}

	return true;
}

bool CListTest::AllocTestVector()
{
	{
		std::vector<int> ii;
		for (int i = 0; i < 1e6; i++) {
			ii.push_back(i);
		}

		TEST_ASSERT(ii.size() == 1e6);

		for (int i = 0; i < 1e6; i++) {
			TEST_ASSERT(ii[i] == i);
		}

		for (int i = 0; i < 1e6; i++) {
			ii.pop_back();
		}

		TEST_ASSERT(ii.size() == 0);
	}

	{
		std::vector<TestObject> ii;
		for (int i = 0; i < 1e6; i++) {
			ii.push_back(TestObject(i,i));
		}

		TEST_ASSERT(ii.size() == 1e6);


		for (int i = 0; i < 1e6; i++) {
			TEST_ASSERT(ii[i] == TestObject(i, i));
		}

		for (int i = 0; i < 1e6; i++) {
			ii.pop_back();
		}

		TEST_ASSERT(ii.size() == 0);
	}

	

	return true;
}
