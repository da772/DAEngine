#include "dapch.h"
#include "physics_bullet3_motion_state.h"

#include "bullet3_physics.h"
#include <glm/gtx/matrix_decompose.hpp>

namespace da::physics
{
	btMotionState* CBullet3MotionState::getMotionState() const
	{
		return m_motionState;
	}

	CBullet3DefaultMotionState::CBullet3DefaultMotionState(const glm::mat4& transform) : CBullet3MotionState(), CPhysicsDefaultMotionState(transform)
	{
		btTransform trnsfrm;

		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(transform, scale, rotation, translation, skew, perspective);

		trnsfrm.setIdentity();
		trnsfrm.setOrigin({ translation.x, translation.y,  translation.z});
		trnsfrm.setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));
		
		m_motionState = new btDefaultMotionState(trnsfrm);
	}

	CBullet3DefaultMotionState::~CBullet3DefaultMotionState()
	{
		ASSERT(m_motionState);
		delete m_motionState;
	}

}