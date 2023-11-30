#pragma once
#include <glm/glm.hpp>

namespace da::maths
{

	class CTransform {

	public:
		void setMat(const glm::mat4& mat);
		glm::mat4& getMat();

	private:
		glm::mat4 m_mat4;

	};
}