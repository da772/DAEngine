#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace da::maths
{

	class CTransform {

	public:
		void setPosition(glm::vec3 pos);
		void setRotation(glm::vec3 rot);

		glm::vec3 getForward() const;
		glm::vec3 getRight() const;
		glm::vec3 getUp() const;
		glm::mat4 getMat() const;

	private:
		glm::vec3 m_pos = { 0.0f, 0.0f, 0.0f };
		glm::quat m_rotation = glm::identity<glm::quat>();
		glm::quat m_invRotation = glm::identity<glm::quat>();

	};
}