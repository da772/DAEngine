#include "dapch.h"
#include "camera.h"
#ifndef DA_TEST
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#endif

namespace da::core
{

	CCamera::CCamera(const Vector3f& position, const Vector3f& rotation)
	{
		calculateMatrix(position, rotation);
	}

	CCamera::CCamera()
	{
		calculateMatrix(Vector3f(0.f), Vector3f(0.f));
	}

	da::Vector3f CCamera::getPosition() const
	{
		return { m_mtx.Mtx[3], m_mtx.Mtx[7], m_mtx.Mtx[11] };
	}

	da::Vector3f CCamera::getRotation() const
	{
		return { m_mtx.Mtx[8], m_mtx.Mtx[9], m_mtx.Mtx[10] };
	}

	da::Vector3f CCamera::getRightVector() const
	{
		return Vector3f(0.f);
	}

	da::Vector3f CCamera::getForwardVector() const
	{
		return Vector3f(0.f);
	}

	void CCamera::calculateMatrix(const Vector3f& position, const Vector3f& rotation)
	{
#ifndef DA_TEST
		glm::vec3 front;
		front.x = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
		front.y = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
		front.z = sin(glm::radians(rotation.x));

		glm::vec3 pos(position.x, position.y, position.z);

		memcpy(m_mtx.Mtx, &glm::lookAt(pos, pos + front, {0.f,0.f,1.f})[0][0], sizeof(float)*4*4);
#endif
	}

	void CCamera::setPosition(const Vector3f& pos)
	{
		calculateMatrix(pos, getRotation());
	}


	void CCamera::setRotation(const Vector3f& rot)
	{
		calculateMatrix(getPosition(), rot);
	}

	const mat4* CCamera::getMatrix() const
	{
		return &m_mtx;
	}

}