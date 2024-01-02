#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace da::maths
{

	class CTransform {

	public:
		CTransform();
		virtual void setPosition(const glm::vec3& pos);
		void offsetPosition(const glm::vec3& delta);
		virtual void setRotation(const glm::vec3& rot);
		void setScale(const glm::vec3& scale);
		void offsetRotation(const glm::vec3& delta);
		glm::vec3 position() const;
		glm::vec3 rotation() const;
		glm::vec3 scale() const;
		virtual glm::mat4 matrix() const;
		glm::vec3 forward() const;
		glm::vec3 up() const;
		glm::vec3 right() const;

	protected:
		glm::vec3 m_position;
		glm::vec3 m_rotation;
		glm::vec3 m_scale = {1.f,1.f,1.f};
		glm::vec3 m_up;
		glm::vec3 m_right;
		glm::vec3 m_forward;

	};
}