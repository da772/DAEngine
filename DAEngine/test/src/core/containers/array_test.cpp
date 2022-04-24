#include "array_test.h"

using namespace da::core::containers;

CArrayTest::CArrayTest()
{

}

void CArrayTest::RunTests()
{
	TEST_FUNC(PrimitiveTest);
	TEST_FUNC(ObjectTest);
}

bool CArrayTest::PrimitiveTest()
{
	TArray<int> m_array = TArray<int>(10, 0xff);
	TEST_ASSERT( m_array.Size() == 10);

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

	TArray<int> newArr(10, 0xef);

	TEST_ASSERT(newArr == m_array);
	newArr = TArray<int>(10, 0xe1);
	TEST_ASSERT(newArr != m_array);
	newArr = TArray<int>(5, 0xef);
	TEST_ASSERT(newArr != m_array);
	newArr = TArray<int>(m_array);
	TEST_ASSERT(newArr == m_array);
	newArr = TArray<int>(std::move(m_array));
	TEST_ASSERT(newArr != m_array);

	m_array = newArr;
	TEST_ASSERT(m_array == newArr);

	m_array = TArray<int>(10, 0xef);
	m_array[9] = 0xff;

	TEST_ASSERT(m_array.Contains(0xef));
	TEST_ASSERT(m_array.Contains(0xff));

	TEST_ASSERT(m_array.Contains(
		[](const int& i) {
			return i == 0xff;
		}
	));

	TEST_ASSERT(!m_array.Contains(
		[](const int& i) {
			return i == 0xf3;
		}
	));

	TEST_ASSERT(*m_array.Find(
		[](const int& i) {
			return i == 0xff;
		}
	) == 0xff );

	TEST_ASSERT(m_array.Find(
		[](const int& i) {
			return i == 0xf3;
		}
	) == m_array.end());

	m_array.Resize(4);
	TEST_ASSERT(m_array.Size() == 4);

	for (const int& i : m_array) {
		TEST_ASSERT(i == 0xef);;
	}

	m_array.Resize(10);

	TEST_ASSERT(m_array.Size() == 10);

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
	TArray<TestObject> m_array(10, TestObject(0, 0));

	for (const TestObject& obj : m_array) {
		TEST_ASSERT(obj == TestObject(0, 0));
	}

	m_array[0].x = 5;
	m_array[0].y = 5;

	TEST_ASSERT(m_array[0] == TestObject(5, 5));
	m_array[9] = TestObject(7, 7);
	TEST_ASSERT(m_array.Contains(TestObject(7, 7)));

	TEST_ASSERT(m_array.Find([](const TestObject& o) {
		return o == TestObject(7, 7);
		}) != m_array.end());

	m_array.Resize(15);

	TEST_ASSERT(m_array.Find([](const TestObject& o) {
		return o == TestObject(-1, -1);
		}) != m_array.end());

	TEnumerator<TestObject> enu = m_array.Find([](const TestObject& o) {
		return o == TestObject(7, 7);
		});

	enu->x = 17;
	enu->y = 24;

	return true;
}
