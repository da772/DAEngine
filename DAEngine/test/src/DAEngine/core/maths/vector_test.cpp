#include "vector_test.h"
#include <DAEngine/core/maths/vector.h>

using namespace da;

CVectorTest::CVectorTest()
{

}

bool CVectorTest::RunTests()
{
	TEST_FUNC(Vec2Tests);
	TEST_FUNC(Vec3Tests);
	TEST_FUNC(Vec4Tests);
	TEST_END();
}

bool CVectorTest::Vec4Tests()
{
	Vector4f vc = { 2,2,2,2 };
	vc += 1;
	TEST_ASSERT((vc == Vector4f(3, 3, 3, 3)));

	vc *= 2;
	TEST_ASSERT((vc == Vector4f(6, 6, 6, 6)));

	vc /= 2;
	TEST_ASSERT((vc == Vector4f(3, 3, 3, 3)));

	vc -= 2;
	TEST_ASSERT(vc == Vector4f(1, 1, 1, 1));

	vc++;
	TEST_ASSERT(vc == Vector4f(2, 2, 2, 2));

	vc--;
	TEST_ASSERT(vc == Vector4f(1, 1, 1, 1));

	TEST_ASSERT(vc.x == vc.r);
	TEST_ASSERT(vc.y == vc.g);
	TEST_ASSERT(vc.z == vc.b);
	TEST_ASSERT(vc.w == vc.a);
	TEST_ASSERT(vc.s == vc.x);
	TEST_ASSERT(vc.t == vc.y);
	TEST_ASSERT(vc.p == vc.z);
	TEST_ASSERT(vc.q == vc.w);


	Vector4f subVec(5, 5, 6, 4);
	Vector4f _subRes = subVec - vc;
	TEST_ASSERT(_subRes == Vector4f(4, 4, 5, 3));
	_subRes = _subRes + Vector4f(2, 2, 2, 2);
	TEST_ASSERT(_subRes == Vector4f(6, 6, 7, 5));

	TEST_ASSERT(Vector4f(1, 2, 3, 4) + Vector2f(2, 3) == Vector4f(3, 5, 3, 4));
	TEST_ASSERT(Vector4f(1, 2, 3, 4) * Vector2f(2, 3) == Vector4f(2, 6, 3, 4));
	TEST_ASSERT(Vector4f(1, 2, 3, 4) - Vector2f(2, 3) == Vector4f(-1, -1, 3, 4));
	TEST_ASSERT(Vector4f(4, 6, 5, 7) / Vector2f(2, 3) == Vector4f(2, 2, 5, 7));
	Vector4f dataTest(3, 1, 6.2f, 8.2f);
	TEST_ASSERT(dataTest.data()[0] == dataTest.x && dataTest.data()[1] == dataTest.y && dataTest.data()[2] == dataTest.z && dataTest.data()[3] == dataTest.w);

	TEST_ASSERT(Vector4f(5, 5, 10, 6) + Vector3f(2, 1, 6) == Vector4f(7, 6, 16, 6));
	TEST_ASSERT(Vector4f(5, 5, 10, 6) - Vector3f(2, 1, 6) == Vector4f(3, 4, 4, 6));
	TEST_ASSERT(Vector4f(5, 5, 10, 6) * Vector3f(2, 1, 6) == Vector4f(10, 5, 60, 6));
	TEST_ASSERT(Vector4f(6, 5, 36, 6) / Vector3f(2, 1, 6) == Vector4f(3, 5, 6, 6));

	Vector4f copyTest = { 2,6,1,9 };
	copyTest += 3;
	TEST_ASSERT(copyTest == Vector4f(5, 9, 4, 12));
	copyTest -= 5;
	TEST_ASSERT(copyTest == Vector4f(0, 4, -1, 7));
	copyTest *= 3;
	TEST_ASSERT(copyTest == Vector4f(0, 12, -3, 21));
	copyTest /= 3;
	TEST_ASSERT(copyTest == Vector4f(0, 4, -1, 7));
	copyTest += copyTest.xy();
	TEST_ASSERT(copyTest == Vector4f(0, 8, -1, 7));
	copyTest += copyTest.xyz();
	TEST_ASSERT(copyTest == Vector4f(0, 16, -2, 7));
	copyTest = 2.f * copyTest;
	TEST_ASSERT(copyTest == Vector4f(0, 32, -4, 14));
	copyTest = copyTest / 2.f;
	TEST_ASSERT(copyTest == Vector4f(0, 16, -2, 7));
	copyTest = .5f + copyTest;
	TEST_ASSERT(copyTest == Vector4f(.5f, 16.5f, -1.5f, 7.5f));
	
	float dot = Vector4f::dot(copyTest, dataTest);
	vc = Vector4f::project(copyTest, dataTest);
	float distance = Vector4f::distance(copyTest, dataTest);
	Vector4f lerp = Vector4f::lerp(copyTest, dataTest, 5);
	vc = Vector4f::reflect(copyTest, dataTest);
	vc = 5.f * vc;
	
	TEST_END();
}

bool CVectorTest::Vec3Tests()
{
	Vector3f vc = { 2,2,2 };
	vc += 1;
	TEST_ASSERT((vc == Vector3f(3, 3, 3)));

	vc *= 2;
	TEST_ASSERT((vc == Vector3f(6, 6, 6)));

	vc /= 2;
	TEST_ASSERT((vc == Vector3f(3, 3, 3)));

	vc -= 2;
	TEST_ASSERT(vc == Vector3f(1, 1, 1));

	vc++;
	TEST_ASSERT(vc == Vector3f(2, 2, 2));

	vc--;
	TEST_ASSERT(vc == Vector3f(1, 1, 1));


	TEST_ASSERT(vc.x == vc.r);
	TEST_ASSERT(vc.y == vc.g);
	TEST_ASSERT(vc.z == vc.b);
	TEST_ASSERT(vc.s == vc.x);
	TEST_ASSERT(vc.t == vc.y);
	TEST_ASSERT(vc.p == vc.z);

	Vector3f subVec(5, 5, 6);
	Vector3f _subRes = subVec - vc;
	TEST_ASSERT(_subRes == Vector3f(4, 4, 5));
	_subRes = _subRes + Vector3f(2, 2, 2);
	TEST_ASSERT(_subRes == Vector3f(6, 6, 7));

	TEST_ASSERT(Vector3f(1, 2, 3) + Vector2f(2, 3) == Vector3f(3, 5, 3));
	TEST_ASSERT(Vector3f(1, 2, 3) * Vector2f(2, 3) == Vector3f(2, 6, 3));
	TEST_ASSERT(Vector3f(1, 2, 3) - Vector2f(2, 3) == Vector3f(-1, -1, 3));
	TEST_ASSERT(Vector3f(4, 6, 5) / Vector2f(2, 3) == Vector3f(2, 2, 5));
	Vector3f dataTest(3, 1, 6.2f);
	TEST_ASSERT(dataTest.data()[0] == dataTest.x && dataTest.data()[1] == dataTest.y && dataTest.data()[2] == dataTest.z);

	TEST_ASSERT(Vector3f(5, 5, 10) + Vector3f(2, 1, 6) == Vector3f(7, 6, 16));
	TEST_ASSERT(Vector3f(5, 5, 10) - Vector3f(2, 1, 6) == Vector3f(3, 4, 4));
	TEST_ASSERT(Vector3f(5, 5, 10) * Vector3f(2, 1, 6) == Vector3f(10, 5, 60));
	TEST_ASSERT(Vector3f(6, 5, 36) / Vector3f(2, 1, 6) == Vector3f(3, 5, 6));

	Vector3f copyTest = { 2,6,1 };
	copyTest += 3;
	TEST_ASSERT(copyTest == Vector3f(5, 9, 4));
	copyTest -= 5;
	TEST_ASSERT(copyTest == Vector3f(0, 4, -1));
	copyTest *= 3;
	TEST_ASSERT(copyTest == Vector3f(0, 12, -3));
	copyTest /= 3;
	TEST_ASSERT(copyTest == Vector3f(0, 4, -1));
	copyTest += copyTest.xy();
	TEST_ASSERT(copyTest == Vector3f(0, 8, -1));
	copyTest = 5.f + copyTest;
	TEST_ASSERT(copyTest == Vector3f(5, 13, 4));
	copyTest = 2.f * copyTest;
	TEST_ASSERT(copyTest == Vector3f(10, 26, 8));

	copyTest = dataTest;
	TEST_ASSERT(copyTest == Vector3f(3, 1, 6.2f));

	copyTest = Vector3f(10, 26, 8);
	
	float dot = Vector3f::dot(copyTest, dataTest);
	vc = Vector3f::project(copyTest, dataTest);
	vc = Vector3f::cross(copyTest, dataTest);
	vc = Vector3f::reflect(copyTest, dataTest);
	vc = Vector3f::lerp(copyTest, dataTest, 5);
	float distance = Vector3f::distance(copyTest, dataTest);
	
	TEST_END();
}

bool CVectorTest::Vec2Tests()
{
	Vector2f vc = { 2,2 };
	vc += 1;
	TEST_ASSERT(vc.x == 3 && vc.y == 3);
	vc *= 2;
	TEST_ASSERT(vc.x == 6 && vc.y == 6);
	vc /= 2;
	TEST_ASSERT(vc.x == 3 && vc.y == 3);
	vc -= 2;
	TEST_ASSERT(vc.x == 1 && vc.y == 1);
	vc++;
	TEST_ASSERT(vc.x == 2 && vc.y == 2);
	vc--;
	TEST_ASSERT(vc.x == 1 && vc.y == 1);

	TEST_ASSERT(vc.x == vc.r);
	TEST_ASSERT(vc.y == vc.g);
	TEST_ASSERT(vc.s == vc.x);
	TEST_ASSERT(vc.t == vc.y);


	Vector2f subVec(5, 5);
	Vector2f _subRes = subVec - vc;
	TEST_ASSERT(_subRes.x == 4 && _subRes.y == 4);
	_subRes = _subRes + Vector2f(2, 2);
	TEST_ASSERT(_subRes.x == 6 && _subRes.y == 6);
	_subRes = _subRes * Vector2f(2, 3);
	TEST_ASSERT(_subRes.x == 12 && _subRes.y == 18);
	_subRes = _subRes / Vector2f(6, 3);
	TEST_ASSERT(_subRes.x == 2 && _subRes.y == 6);
	vc -= Vector2f(2, 3);
	TEST_ASSERT(vc.x == -1 && vc.y == -2);
	vc += Vector2f(4, 6);
	TEST_ASSERT(vc.x == 3 && vc.y == 4);
	vc *= Vector2f(2, 3);
	TEST_ASSERT(vc.x == 6 && vc.y == 12);
	vc /= Vector2f(6, 4);
	TEST_ASSERT(vc.x == 1 && vc.y == 3);
	vc = vc * 6;
	TEST_ASSERT(vc.x == 6 && vc.y == 18);
	vc = vc / 3;
	TEST_ASSERT(vc.x == 2 && vc.y == 6);
	vc = vc + 2;
	TEST_ASSERT(vc.x == 4 && vc.y == 8);
	vc = vc - 4;
	TEST_ASSERT(vc.x == 0 && vc.y == 4);
	vc = 5.f + vc;
	TEST_ASSERT(vc == Vector2f(5, 9));
	vc = 2.f * vc;
	TEST_ASSERT(vc == Vector2f(10, 18));
	vc += 3;
	
	float dot = Vector2f::dot(vc, _subRes);
	vc = Vector2f::project(vc, _subRes);
	vc = Vector2f::reflect(_subRes, vc);
	vc = Vector2f::lerp(_subRes, vc, 5);
	float distance = Vector2f::distance(_subRes, vc);
	
	TEST_END();
}
