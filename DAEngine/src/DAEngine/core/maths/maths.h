#pragma once

#include "vector.h"

namespace da::core::maths
{
	bool IsPrime(const int& n);
	int NextPrime(const int& n);
	int PreviousPrime(const int& n);
	inline double wrapAngle(double angle)
	{
		double twoPi = 2.0 * 3.141592865358979;
		return angle - twoPi * floor(angle / twoPi);
	}
	glm::quat calculateRotationForDirection(const glm::vec3& direction);
	glm::vec3 calculateEulerRotationForDirection(const glm::vec3& direction);
}