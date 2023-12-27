#include "dapch.h"
#include "camera.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace da::core
{
	da::core::CCamera* CCamera::s_camera;


	CCamera::CCamera()
	{
		setPosition({ 0,0,0 });
		setRotation({ 0,0,0 });
	}

	void CCamera::zoom(float offset)
	{
		fov = glm::clamp(fov - offset, MIN_FOV, MAX_FOV);
	}

	void CCamera::setRotation(const glm::vec3& rot)
	{
		CTransform::setRotation(rot);
		updateVectors();
	}

	void CCamera::lookAt(const glm::vec3& target)
	{
		m_forward = glm::normalize(target - m_position);
		m_right = glm::normalize(glm::cross(m_forward, m_up));
		m_up = glm::cross(m_right, m_forward);
		m_rotation = glm::degrees(glm::eulerAngles(glm::quat_cast(glm::lookAt(m_position, target, m_up))));
		updateVectors();
	}

	da::core::CCamera* CCamera::getCamera()
	{
		return s_camera;
	}

	void CCamera::setCamera(CCamera* camera)
	{
		s_camera = camera;
	}

	glm::mat4 CCamera::matrix() const
	{
		return glm::lookAt(m_position, m_position + m_forward, m_up);
	}

	void CCamera::updateVectors()
	{
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f))

			* glm::rotate(glm::mat4(1.0f), glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		m_forward = glm::vec3(rotationMatrix * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
		m_right = glm::vec3(rotationMatrix * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
		m_up = glm::vec3(rotationMatrix * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));

		// Ensure vectors are orthogonalized
		m_right = -glm::normalize(m_right);
		m_up = glm::normalize(m_up);
		m_forward = glm::cross(-m_right, m_up);
	}
}