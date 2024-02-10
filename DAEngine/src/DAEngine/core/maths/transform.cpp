#include "dapch.h"
#include "transform.h"

#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp> >
#include <glm/gtx/matrix_decompose.hpp>

namespace da::maths
{
	CTransform::CTransform()
	{
		setPosition({ 0,0,0 });
		setRotation({ 0,0,0 });
	}

	void CTransform::setPosition(const glm::vec3& pos)
	{
		m_position = pos;
		m_dirty = true;
	}

	void CTransform::offsetPosition(const glm::vec3& delta)
	{
		setPosition(m_position + delta);
		m_dirty = true;
	}

	void CTransform::setRotation(const glm::vec3& rot)
	{
		m_rotation = glm::quat(glm::radians(rot));
		m_dirty = true;
	}

	void CTransform::setRotation(const glm::quat& rot)
	{
		m_rotation = rot;
		m_dirty = true;
	}

	void CTransform::offsetRotation(const glm::vec3& delta)
	{
		glm::vec3 currentEulerAngles = glm::degrees(glm::eulerAngles(m_rotation));

		currentEulerAngles += delta;
		currentEulerAngles.y = 0.0f;
		m_rotation = glm::quat(glm::radians(currentEulerAngles));

		setRotation(glm::normalize(m_rotation));


		m_dirty = true;
	}

	glm::vec3 CTransform::position() const
	{
		return m_position;
	}


	glm::quat CTransform::rotation() const
	{
		return m_rotation;
	}


	glm::vec3 CTransform::rotationEuler() const
	{
		return glm::degrees(glm::eulerAngles(m_rotation));
	}

	const glm::mat4& CTransform::matrix()
	{
		if (m_dirty) {
			m_dirty = false;
			glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), m_position);
			glm::mat4 rotationMatrix = glm::toMat4(m_rotation);
			glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0), m_scale);
			m_mat = translationMatrix * rotationMatrix * scaleMatrix;;
		}
		
		return m_mat;
	}

	glm::vec3 CTransform::forward() const
	{
		return -m_forward;
	}

	glm::vec3 CTransform::up() const
	{
		return m_up;
	}

	glm::vec3 CTransform::right() const
	{
		return m_right;
	}

	void CTransform::setScale(const glm::vec3& scale)
	{
		m_scale = scale;
		m_dirty = true;
	}

	glm::vec3 CTransform::scale() const
	{
		return m_scale;
	}

	void CTransform::setTransform(const glm::mat4& transform)
	{
		glm::vec3 scale, translation, skew;
		glm::quat orientation;
		glm::vec4 prespective;

		glm::decompose(transform, scale, orientation, translation, skew, prespective);

		setPosition(translation);
		setRotation(orientation);
	}

	bool CTransform::isDirty() const
	{
		return m_dirty;
	}

}