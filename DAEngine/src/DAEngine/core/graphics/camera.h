#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "daengine/core/maths/transform.h"

namespace da::core
{
	class CCamera : public da::maths::CTransform{

	public:

		CCamera();
		float fov = 73.7397953f;

		float zNear = 1.f;
		float zFar = 10000.0f;

		float exposure = 1.f;

		virtual const glm::mat4& matrix() override;
		virtual void setRotation(const glm::vec3& rot) override;

		void zoom(float offset);
		void lookAt(const glm::vec3& position);

		static CCamera* getCamera();
		static void setCamera(CCamera* camera);

	private:
		void updateVectors();
		static constexpr float MIN_FOV = 10.0f;
		static constexpr float MAX_FOV = 90.0f;

		static CCamera* s_camera;

	};
}