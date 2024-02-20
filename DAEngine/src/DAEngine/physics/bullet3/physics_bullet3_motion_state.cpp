#include "dapch.h"
#include "physics_bullet3_motion_state.h"

#include "bullet3_physics.h"
#include <glm/gtx/matrix_decompose.hpp>
#include "DAEngine/core/ecs/entity.h"

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

	CBullet3EntityMotionState::CBullet3EntityMotionState(da::core::CEntity* entity) : CPhysicsEntityMotionState(entity)
	{
		m_motionState = this;
		m_finalTransform = entity->getTransform().matrix();
	}

	void CBullet3EntityMotionState::getWorldTransform(btTransform& worldTrans) const
	{
		worldTrans.setFromOpenGLMatrix(glm::value_ptr(m_finalTransform));
	}

	void CBullet3EntityMotionState::setWorldTransform(const btTransform& worldTrans)
	{
		glm::mat4 transform;
		worldTrans.getOpenGLMatrix(glm::value_ptr(transform));
		m_entity->getTransform().setTransform(transform);
		m_finalTransform = transform;
	}

	CBullet3EntityMotionState::~CBullet3EntityMotionState()
	{

	}

}