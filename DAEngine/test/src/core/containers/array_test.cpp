#include "array_test.h"
#include <DAEngine/core/containers.h>

using namespace da;

CArrayTest::CArrayTest()
{

}

bool CArrayTest::RunTests()
{
	TEST_FUNC(PrimitiveTest);
	TEST_FUNC(ObjectTest);
	TEST_END();
}

bool CArrayTest::PrimitiveTest()
{
	Array<int> m_array = Array<int>(10, 0xff);
	TEST_ASSERT( m_array.size() == 10);

	int counter = 0;

	for (const int& i : m_array) {
		counter++;
		TEST_ASSERT(i == 0xff);
	}

	for (size_t i = 0; i < 10; i++) {
		TEST_ASSERT(m_array[i] == 0xff);
		m_array[i] = 0xef;
		TEST_ASSERT(m_array[i] == 0xef);
	}

	Array<int> newArr(10, 0xef);
	TEST_ASSERT(newArr == m_array);
	newArr = Array<int>(10, 0xe1);
	TEST_ASSERT(newArr != m_array);
	newArr = Array<int>(5, 0xef);
	TEST_ASSERT(newArr != m_array);
	newArr = Array<int>(m_array);
	TEST_ASSERT(newArr == m_array);
	newArr = Array<int>(std::move(m_array));
	TEST_ASSERT(newArr != m_array);


	m_array = newArr;
	TEST_ASSERT(m_array == newArr);

	m_array = Array<int>(10, 0xef);
	m_array[9] = 0xff;

	TEST_ASSERT(m_array.contains(0xef));
	TEST_ASSERT(m_array.contains(0xff));

	TEST_ASSERT(m_array.contains(
		[](const int& i) {
			return i == 0xff;
		}
	));

	TEST_ASSERT(!m_array.contains(
		[](const int& i) {
			return i == 0xf3;
		}
	));

	TEST_ASSERT(*m_array.find(
		[](const int& i) {
			return i == 0xff;
		}
	) == 0xff );

	TEST_ASSERT(m_array.find(
		[](const int& i) {
			return i == 0xf3;
		}
	) == m_array.end());

	m_array.resize(4);
	TEST_ASSERT(m_array.size() == 4);

	for (const int& i : m_array) {
		TEST_ASSERT(i == 0xef);;
	}

	m_array.resize(10);

	TEST_ASSERT(m_array.size() == 10);

	for (int i = 0; i < 4; i++) {
		TEST_ASSERT(m_array[i] == 0xef);;
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

bool CArrayTest::ObjectTest()
{
	Array<TestObject> m_array(10, TestObject(0, 0));

	for (const TestObject& obj : m_array) {
		TEST_ASSERT(obj == TestObject(0, 0));
	}

	m_array[0].x = 5;
	m_array[0].y = 5;

	TEST_ASSERT(m_array[0] == TestObject(5, 5));
	m_array[9] = TestObject(7, 7);
	TEST_ASSERT(m_array.contains(TestObject(7, 7)));

	TEST_ASSERT(m_array.find([](const TestObject& o) {
		return o == TestObject(7, 7);
		}) != m_array.end());

	m_array.resize(15);

	TEST_ASSERT(m_array.find([](const TestObject& o) {
		return o == TestObject(-1, -1);
		}) != m_array.end());

	Enumerator<TestObject> enu = m_array.find([](const TestObject& o) {
		return o == TestObject(7, 7);
		});

	enu->x = 17;
	enu->y = 24;

	return true;
}
