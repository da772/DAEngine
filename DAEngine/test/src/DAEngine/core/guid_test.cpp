#include "guid_test.h"

#include <DAEngine/core/guid.h>
#include <DAEngine/core/containers.h>

using namespace da::core;

CGuidTest::CGuidTest()
{

}

bool CGuidTest::RunTests()
{
	TEST_BEGIN();
	TEST_FUNC(GuidTests);
	TEST_END();
}

bool CGuidTest::GuidTests()
{
	CGuid guid = CGuid();

	TEST_ASSERT(strcmp(guid.c_str(), "00000000-0000-0000-0000-000000000000") == 0);

	TEST_ASSERT(!guid.isValid());

	guid = CGuid::Generate();

	TEST_ASSERT(guid.isValid());

	for (int i = 0; i < 1e3; i++) {
		CGuid guid1 = CGuid::Generate();
		CGuid guid2 = CGuid::Generate();
		TEST_ASSERT(guid1.isValid());
		TEST_ASSERT(guid2.isValid());
		TEST_ASSERT(CGuid(guid1.c_str()) == guid1);
		TEST_ASSERT(CGuid(guid2.c_str()) != guid1);
		TEST_ASSERT(CGuid(guid2.c_str()) == guid2);
	}

	TEST_ASSERT(guid == CGuid(guid));
	TEST_ASSERT(guid == CGuid(guid.data()));

	guid = CGuid("29000000-2348-0000-be18-000084670000");

	TEST_ASSERT(strcmp(guid.c_str(), "29000000-2348-0000-be18-000084670000") == 0);

	return true;
}