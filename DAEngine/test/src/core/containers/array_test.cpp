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
	m_array = TArray<int>(10, 0xff);
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


	//TODO: Move this to IEnumerable Test Case
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

	TEST_ASSERT(!m_array.Find(
		[](const int& i) {
			return i == 0xf3;
		}
	));

	return true;
}

bool CArrayTest::ObjectTest()
{
	// TODO: Create test class type for this object test, test both stack objects and heap pointer refs
	return true;
}
