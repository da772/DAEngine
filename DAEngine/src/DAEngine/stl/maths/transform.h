#pragma once

#include "daglm.h"
#include "dastd.h"

namespace da
{

	class CTransform {

	public:
		CTransform();
		virtual void setPosition(const glm::vec3& pos);
		void offsetPosition(const glm::vec3& delta);
		virtual void setRotation(const glm::vec3& rot);
		virtual void setRotation(const glm::quat& rot);
		void setScale(const glm::vec3& scale);
		void offsetRotation(const glm::vec3& delta);
		void setTransform(const glm::mat4& transform);
		glm::vec3 position() const;
		glm::vec3 rotationEuler() const;
		glm::quat rotation() const;
		glm::vec3 scale() const;
		virtual const glm::mat4& matrix();
		glm::vec3 forward() const;
		glm::vec3 up() const;
		glm::vec3 right() const;
		bool isDirty() const;

		void addOnTransform(const std::function<void(const glm::mat4& oldT, const glm::mat4&)>& f);
		void removeOnTransform(const std::function<void(const glm::mat4& oldT, const glm::mat4&)>& f);

	protected:
		glm::vec3 m_position;
		glm::quat m_rotation;
		glm::vec3 m_scale = {1.f,1.f,1.f};
		glm::vec3 m_up;
		glm::vec3 m_right;
		glm::vec3 m_forward;

		glm::mat4 m_mat = glm::mat4(1);

		bool m_dirty = true;

	private:
		std::vector <std::function<void(const glm::mat4& oldT, const glm::mat4& newT)>> m_callbacks;
	};
}