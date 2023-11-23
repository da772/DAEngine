#include "dapch.h"
#include "camera.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace da::core
{
	const glm::vec3 CCamera::X = { 1.0f, 0.0f, 0.0f };
	const glm::vec3 CCamera::Y = { 0.0f, 1.0f, 0.0f };
	const glm::vec3 CCamera::Z = { 0.0f, 0.0f, 1.0f };

	da::core::CCamera* CCamera::s_camera;

	void CCamera::move(glm::vec3 delta)
	{
		m_pos += delta;
	}

	void CCamera::rotate(glm::vec2 delta)
	{
		delta = glm::radians(delta);

		// limit pitch
		float dot = glm::dot(m_upAxis, forward());
		if ((dot < -0.99f && delta.x < 0.0f) || // angle nearing 180 degrees
			(dot > 0.99f && delta.x > 0.0f))    // angle nearing 0 degrees
			delta.x = 0.0f;

		// pitch is relative to current sideways m_rotation
		// yaw happens independently
		// this prevents roll
		m_rotation = glm::rotate(glm::identity<glm::quat>(), delta.x, X) *           // pitch
			m_rotation * glm::rotate(glm::identity<glm::quat>(), delta.y, Y); // yaw
		// normalize?
		m_invRotation = glm::conjugate(m_rotation);
	}

	void CCamera::zoom(float offset)
	{
		fov = glm::clamp(fov - offset, MIN_FOV, MAX_FOV);
	}

	void CCamera::lookAt(const glm::vec3& m_position, const glm::vec3& target, const glm::vec3& up)
	{
		m_pos = m_position;
		m_upAxis = up;

		// model m_rotation
		// maps vectors to camera space (x, y, z)
		glm::vec3 forward = glm::normalize(target - m_position);
		m_rotation = glm::rotation(forward, Z);

		// correct the up vector
		// the cross product of non-orthogonal vectors is not normalized
		glm::vec3 right = glm::normalize(glm::cross(glm::normalize(up), forward)); // left-handed coordinate system
		glm::vec3 orthUp = glm::cross(forward, right);
		glm::quat upm_rotation = glm::rotation(m_rotation * orthUp, Y);
		m_rotation = upm_rotation * m_rotation;

		// inverse of the model m_rotation
		// maps camera space vectors to model vectors
		m_invRotation = glm::conjugate(m_rotation);
	}

	glm::vec3 CCamera::position() const
	{
		return m_pos;
	}

	glm::mat4 CCamera::matrix() const
	{
		return glm::toMat4(m_rotation) * glm::translate(glm::identity<glm::mat4>(), -m_pos);
	}

	glm::vec3 CCamera::forward() const
	{
		return m_invRotation * Z;
	}

	glm::vec3 CCamera::up() const
	{
		return m_invRotation * Y;
	}

	glm::vec3 CCamera::right() const
	{
		return m_invRotation * X;
	}

	da::core::CCamera* CCamera::getCamera()
	{
		return s_camera;
	}

	void CCamera::setCamera(CCamera* camera)
	{
		s_camera = camera;
	}

}