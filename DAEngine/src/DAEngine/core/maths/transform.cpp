#include "dapch.h"
#include "transform.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace da::maths
{
	
	static const glm::vec3 X = { 1.0f, 0.0f, 0.0f };
	static const glm::vec3 Y = { 0.0f, 1.0f, 0.0f };
	static const glm::vec3 Z = { 0.0f, 0.0f, 1.0f };

	void CTransform::setPosition(glm::vec3 pos)
	{
		m_pos = pos;
	}

	void CTransform::setRotation(glm::vec3 delta)
	{
		delta = glm::radians(delta);

		// limit pitch
		float dot = glm::dot(Y, getForward());
		if ((dot < -0.99f && delta.x < 0.0f) || // angle nearing 180 degrees
			(dot > 0.99f && delta.x > 0.0f))    // angle nearing 0 degrees
			delta.x = 0.0f;

		// pitch is relative to current sideways m_rotation
		// yaw happens independently
		// this prevents roll
		m_rotation = glm::rotate(glm::identity<glm::quat>(), delta.x, X) *           // pitch
			m_rotation * glm::rotate(glm::identity<glm::quat>(), delta.y, Y) * // yaw
			m_rotation* glm::rotate(glm::identity<glm::quat>(), delta.z, Z); // roll
		// normalize?
		m_invRotation = glm::conjugate(m_rotation);
	}

	glm::vec3 CTransform::getForward() const
	{
		return m_invRotation * X;
	}

	glm::vec3 CTransform::getRight() const
	{
		return m_invRotation * Y;
	}

	glm::vec3 CTransform::getUp() const
	{
		return m_invRotation * Z;
	}

	glm::mat4 CTransform::getMat() const
	{
		return glm::toMat4(m_rotation) * glm::translate(glm::identity<glm::mat4>(), -m_pos);
	}

}