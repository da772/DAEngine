
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

	void CCamera::setRotation(const glm::quat& rot)
	{
		CTransform::setRotation(rot);
		updateVectors();
	}

	void CCamera::lookAt(const glm::vec3& target)
	{
		// Compute the forward vector from the camera position to the target position
		m_forward = glm::normalize(target - m_position);

		// Handle the case where forward vector is zero
		if (glm::length(m_forward) < glm::epsilon<float>() || glm::all(glm::isnan(m_forward))) {
			m_forward = { 0.f,1.f,0.f };
			m_right = { 1.f,0.f,0.f };
			m_up = { 0.f,0.f,1.f };
			return;
		}

		// Compute the right vector
		m_right = glm::normalize(glm::cross(m_forward, {0.f,0.f,1.f}));

		// Handle the case where forward vector is parallel to worldUp vector
		if (glm::length(m_right) < glm::epsilon<float>() || glm::all(glm::isnan(m_right))) {
			// Adjust worldUp slightly
			glm::vec3 adjustedUp = glm::vec3(0.f,0.f,1.f) + glm::vec3(0.001f, 0.0f, 0.0f);
			m_right = glm::normalize(glm::cross(m_forward, adjustedUp));
		}

		// Recompute the orthogonal up vector
		m_up = glm::cross(m_right, m_forward);
		// Create the rotation matrix
		glm::mat3 rotationMatrix(m_right, m_forward, m_up);

		// Convert the rotation matrix to a quaternion
		m_rotation = glm::quat_cast(rotationMatrix);
	}

	glm::quat CCamera::lookAt(const glm::vec3& target, const glm::vec3& position)
	{
		// Compute the forward vector from the camera position to the target position
		glm::vec3 forward = glm::normalize(target - position);
		glm::vec3 right = glm::vec3(0.f);
		glm::vec3 up = glm::vec3(0.f);

		// Handle the case where forward vector is zero
		if (glm::length(forward) < glm::epsilon<float>() || glm::all(glm::isnan(forward))) {
			forward = { 0.f,1.f,0.f };
			right = { 1.f,0.f,0.f };
			up = { 0.f,0.f,1.f };
			glm::mat3 rotationMatrix(right, forward, up);
			return glm::quat_cast(rotationMatrix);
		}

		// Compute the right vector
		right = glm::normalize(glm::cross(forward, { 0.f,0.f,1.f }));

		// Handle the case where forward vector is parallel to worldUp vector
		if (glm::length(right) < glm::epsilon<float>() || glm::all(glm::isnan(right))) {
			// Adjust worldUp slightly
			glm::vec3 adjustedUp = glm::vec3(0.f, 0.f, 1.f) + glm::vec3(0.001f, 0.0f, 0.0f);
			right = glm::normalize(glm::cross(forward, adjustedUp));
		}

		// Recompute the orthogonal up vector
		up = glm::cross(right, forward);
		// Create the rotation matrix
		glm::mat3 rotationMatrix(right, forward, up);

		// Convert the rotation matrix to a quaternion
		return glm::quat_cast(rotationMatrix);
	}

	da::core::CCamera* CCamera::getCamera()
	{
		return s_camera;
	}

	void CCamera::setCamera(CCamera* camera)
	{
		s_camera = camera;
	}

	const glm::mat4& CCamera::matrix()
	{
		if (m_dirty) {
			m_mat = glm::lookAt(m_position, m_position + m_forward, m_up);
			m_dirty = false;
		}
		
		return m_mat;
	}

	void CCamera::updateVectors()
	{
		m_forward = glm::rotate(m_rotation, glm::vec3(0.0f, 1.0f, 0.0f));
		m_right = glm::cross(m_forward, glm::vec3(0.0f, 0.0f, 1.0f));
		m_up = glm::cross(m_right, m_forward);

		// Ensure vectors are orthogonalized
		m_right = glm::normalize(m_right);
		m_up = glm::normalize(m_up);
		m_forward = glm::normalize(m_forward);
	}
}