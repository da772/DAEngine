
#include "maths.h"
#include <math.h> 

namespace da::core::maths
{
	bool IsPrime(const int& n)
	{
		int sqr = (int)sqrt(n);
		for (int i = 2; i <= sqr; i++) {
			if (n % i == 0)
				return false;
		}
		return true;
	}

	int NextPrime(const int& n)
	{
		int prime = n + 1;
		while (!IsPrime(prime)) {
			prime++;
		}
		return prime;
	}

	int PreviousPrime(const int& n)
	{
		int prime = n - 1;
		while (!IsPrime(prime)) {
			prime--;
		}
		return prime;
	}

	glm::quat calculateRotationForDirection(const glm::vec3& direction)
	{
		// Normalize the direction vector
		glm::vec3 normDir = glm::normalize(direction);

		// Define the reference forward direction (you can change this to any direction you need)
		glm::vec3 forward = glm::vec3(0.0f, 1.0f, 0.f);

		// Calculate the rotation axis
		glm::vec3 rotationAxis = glm::cross(forward, normDir);
		float rotationAngle = glm::acos(glm::dot(forward, normDir));

		// Handle the edge case when direction is the same as forward (no rotation needed)
		if (glm::length(rotationAxis) < 0.0001f) {
			// If direction and forward are opposite, we need to rotate 180 degrees around any perpendicular axis
			if (glm::dot(forward, normDir) < -0.9999f) {
				rotationAxis = glm::vec3(1.0f, 0.0f, 0.0f);
				rotationAngle = glm::pi<float>();
			}
			else {
				// No rotation needed
				return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
			}
		}

		// Normalize the rotation axis
		rotationAxis = glm::normalize(rotationAxis);

		// Create the quaternion representing the rotation
		glm::quat quaternion = glm::angleAxis(rotationAngle, rotationAxis);

		return quaternion;
	}

	glm::vec3 calculateEulerRotationForDirection(const glm::vec3& direction)
	{
		return glm::eulerAngles(calculateRotationForDirection(direction));
	}

}