#include "dapch.h"
#include "transform.h"

namespace da::maths
{
	void CTransform::setMat(const glm::mat4& mat)
	{
		m_mat4 = mat;
	}

	glm::mat4& CTransform::getMat()
	{
		return m_mat4;
	}
}