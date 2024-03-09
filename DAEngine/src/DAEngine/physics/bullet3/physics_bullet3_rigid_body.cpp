#include "dapch.h"
#include "physics_bullet3_rigid_body.h"
#include "../physics.h"
#include "bullet3_physics.h"
#include "physics_bullet3_motion_state.h"
#include "physics_bullet3_shape.h"

namespace da::physics
{
	CBullet3RigidBody::CBullet3RigidBody(IPhysicsShape* shape, IPhysicsMotionState* state, float mass, const glm::vec3& inertia)
		: IPhysicsRigidBody(shape, state, mass, inertia), m_linearFactor(1.f), m_angularFactor(1.f), m_gravityFactor(1.f)
	{
		ASSERT(shape);
		ASSERT(state);

		CBullet3Physics* physics = dynamic_cast<CBullet3Physics*>(CPhysics::getPhysicsType());
		ASSERT(physics);

		CBullet3MotionState* motionState = dynamic_cast<CBullet3MotionState*>(state);
		ASSERT(motionState);

		CBullet3Shape* b3shape = dynamic_cast<CBullet3Shape*>(shape);
		ASSERT(b3shape);

		btVector3 b3Intertia(inertia.x, inertia.y, inertia.z);
		if (mass != 0.f)
			b3shape->getShape()->calculateLocalInertia(mass, b3Intertia);

		btRigidBody::btRigidBodyConstructionInfo info(mass, motionState->getMotionState(), b3shape->getShape(), b3Intertia);
		m_rigidBody = new btRigidBody(info);
		physics->getDynamicsWorld()->addRigidBody(m_rigidBody);
		m_rigidBody->forceActivationState(DISABLE_DEACTIVATION);
	}

	glm::mat4 CBullet3RigidBody::getTransform() const
	{
		ASSERT(m_rigidBody);
		glm::mat4 out;
		m_rigidBody->getWorldTransform().getOpenGLMatrix(glm::value_ptr(out));
		return out;
	}

	void CBullet3RigidBody::setTransform(const glm::mat4& transform)
	{
		ASSERT(m_rigidBody);
		btTransform trnsform;
		setActivationState(EPhysicsActivationState::ActiveTag);
		trnsform.setFromOpenGLMatrix(glm::value_ptr(transform));
		m_rigidBody->setWorldTransform(trnsform);
	}

	void CBullet3RigidBody::applyImpulse(const glm::vec3& impulse)
	{
		ASSERT(m_rigidBody);
		setActivationState(EPhysicsActivationState::ActiveTag);
		m_rigidBody->activate(true);
		m_rigidBody->applyCentralImpulse({ impulse.x, impulse.y, impulse.z });
	}

	void CBullet3RigidBody::applyImpulse(const glm::vec3& impulse, const glm::vec3& location)
	{
		ASSERT(m_rigidBody);
		setActivationState(EPhysicsActivationState::ActiveTag);
		m_rigidBody->activate(true);
		m_rigidBody->applyImpulse({ impulse.x, impulse.y, impulse.z }, {location.x, location.y, location.z});
	}

	void CBullet3RigidBody::setActivationState(EPhysicsActivationState state)
	{
		ASSERT(m_rigidBody);
		m_rigidBody->setActivationState((int)state);
	}

	void CBullet3RigidBody::setRestitution(float restitution)
	{
		ASSERT(m_rigidBody);
		m_rigidBody->setRestitution(restitution);
	}

	class btRigidBody* CBullet3RigidBody::getRigidBody() const
	{
		return m_rigidBody;
	}

	CBullet3RigidBody::~CBullet3RigidBody()
	{
		CBullet3Physics* physics = dynamic_cast<CBullet3Physics*>(CPhysics::getPhysicsType());
		ASSERT(physics);
		physics->getDynamicsWorld()->removeRigidBody(m_rigidBody);
		ASSERT(m_rigidBody->getMotionState());
		delete m_rigidBody->getMotionState();
		ASSERT(m_rigidBody->getCollisionShape());
		delete m_rigidBody->getCollisionShape();
		ASSERT(m_rigidBody);
		delete m_rigidBody;
	}

	void CBullet3RigidBody::setUserData(void* ptr)
	{
		ASSERT(m_rigidBody);
		m_rigidBody->setUserPointer(ptr);
	}

	void CBullet3RigidBody::setLinearVelocity(const glm::vec3& impulse)
	{
		ASSERT(m_rigidBody);
		m_rigidBody->setLinearVelocity({impulse.x, impulse.y, impulse.z});
	}

	const glm::vec3& CBullet3RigidBody::getLinearVelocity() const
	{
		ASSERT(m_rigidBody);
		btVector3 velocity = m_rigidBody->getLinearVelocity();
		return { velocity.x(), velocity.y(), velocity.z() };
	}

	void CBullet3RigidBody::setAngularVelocity(const glm::vec3& impulse)
	{
		ASSERT(m_rigidBody);
		m_rigidBody->setAngularVelocity({ impulse.x, impulse.y, impulse.z });
	}

	const glm::vec3& CBullet3RigidBody::getAngularVelocity() const
	{
		ASSERT(m_rigidBody);
		btVector3 velocity = m_rigidBody->getAngularVelocity();
		return { velocity.x(), velocity.y(), velocity.z()};
	}

	void CBullet3RigidBody::setAngularFactor(const glm::vec3& factor)
	{
		ASSERT(m_rigidBody);
		m_angularFactor = factor;
		m_rigidBody->setAngularFactor({ factor.x, factor.y, factor.z });
	}

	void CBullet3RigidBody::setGravityFactor(const glm::vec3& factor)
	{
		ASSERT(m_rigidBody);
		m_gravityFactor = factor;
		m_rigidBody->setGravity({ factor.x, factor.y, factor.z });
	}

	void CBullet3RigidBody::setLinearFactor(const glm::vec3& factor)
	{
		ASSERT(m_rigidBody);
		m_linearFactor = factor;
		m_rigidBody->setLinearFactor({ factor.x, factor.y, factor.z });
	}

	const glm::vec3& CBullet3RigidBody::getLinearFactor() const
	{
		return m_linearFactor;
	}

	const glm::vec3& CBullet3RigidBody::getGravityFactor() const
	{
		return m_gravityFactor;
	}

	const glm::vec3& CBullet3RigidBody::getAngularFactor() const
	{
		return m_angularFactor;
	}

#if defined(DA_DEBUG) || defined(DA_RELEASE)
	void CBullet3RigidBody::debugDraw()
	{
		ASSERT(m_rigidBody);
		CBullet3Physics* physics = dynamic_cast<CBullet3Physics*>(CPhysics::getPhysicsType());
		ASSERT(physics);
		CBullet3Shape* shape = dynamic_cast<CBullet3Shape*>(m_shape);
		ASSERT(shape);

		physics->getDynamicsWorld()->debugDrawObject(m_rigidBody->getWorldTransform(), shape->getShape(), { 1.f, 0.f,0.f });
	}

#endif

}