
#include "physics_bullet3_motion_state.h"

#include "bullet3_physics.h"
#include "core/ecs/entity.h"
#include <app/ecs/rigid_body_component.h>
#include <core/events/event_handler.h>
#include <glm/gtx/matrix_decompose.hpp>
#include "physics/physics_rigid_body.h"

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
		ASSERT(m_entity);
		m_motionState = this;
		m_finalTransform = entity->getTransform().matrix();
		onTransform(m_finalTransform, m_finalTransform);
		m_entity->getTransform().addOnTransform(BIND_EVENT_FN_2(CBullet3EntityMotionState, onTransform));
	}

	void CBullet3EntityMotionState::getWorldTransform(btTransform& worldTrans) const
	{
		worldTrans.setFromOpenGLMatrix(glm::value_ptr(m_finalTransform));
	}

	void CBullet3EntityMotionState::setWorldTransform(const btTransform& worldTrans)
	{
		glm::mat4 transform;
		worldTrans.getOpenGLMatrix(glm::value_ptr(transform));
		m_localChange = true;
		m_entity->getTransform().setTransform(transform);
		m_finalTransform = transform;
	}

	void CBullet3EntityMotionState::onTransform(const glm::mat4& oldTransform, const glm::mat4& newTransform)
	{
		if (m_localChange)
		{
			m_localChange = false;
			return;
		}

		btTransform transform;
		transform.setFromOpenGLMatrix(glm::value_ptr(newTransform));
		m_motionState->setWorldTransform(transform);
		da::core::FComponentRef<da::core::CRigidBodyComponent> rb = m_entity->getComponent<da::core::CRigidBodyComponent>();

		if (!rb.isValid())
		{
			return;
		}

		rb->getPhysicsBody()->setTransform(newTransform);
	}

	CBullet3EntityMotionState::~CBullet3EntityMotionState()
	{
		ASSERT(m_entity);
		m_entity->getTransform().removeOnTransform(BIND_EVENT_FN_2(CBullet3EntityMotionState, onTransform));
	}

	

}