#pragma once


#include "daglm.h"
#include "dastd.h"

namespace da
{
	bool IsPrime(const int& n);
	int NextPrime(const int& n);
	int PreviousPrime(const int& n);
	inline double wrapAngle(double angle)
	{
		const double TWO_PI = 2.0 * 3.141592865358979;
		if (std::abs(angle) <= .0001)
		{
			return 0.0000;
		}
		angle = fmod(angle, TWO_PI);
		if (angle < 0) {
			angle += TWO_PI;
		}
		return angle;
	}
	glm::quat calculateRotationForDirection(const glm::vec3& direction);
	glm::vec3 calculateEulerRotationForDirection(const glm::vec3& direction);

	constexpr float lerp(float a, float b, float f)
	{
		return a * (1.0 - f) + (b * f);
	}
}