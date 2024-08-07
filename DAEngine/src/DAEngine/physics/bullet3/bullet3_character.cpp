#include "dapch.h"

#include "bullet3_character.h"
#include "physics_bullet3_rigid_body.h"
#include "bullet3_physics.h"
#include "physics_bullet3_shape.h"


namespace da::physics
{
	CBullet3Character::CBullet3Character(const FCharacterTuning& data, IPhysicsShape& shape) : ICharacterController(data, shape), m_characterController(0, 0, 0), m_ghost()
	{
		CBullet3Physics* physics = dynamic_cast<CBullet3Physics*>(CPhysics::getPhysicsType());
		ASSERT(physics);

		CBullet3Shape* b3Shape = dynamic_cast<CBullet3Shape*>(&shape);
		ASSERT(b3Shape);
		btConvexShape* convexShape = dynamic_cast<btConvexShape*>(b3Shape->getShape());
		ASSERT(convexShape);

		m_ghost.setCollisionShape(convexShape);
		m_ghost.setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);

		btTransform startTransform;
		startTransform.setIdentity();
		startTransform.setOrigin(btVector3(-2, 1, 3));
		m_ghost.setWorldTransform(startTransform);

		m_characterController = btKinematicCharacterController(&m_ghost, convexShape, data.StepHeight, {0.f,0.f,1.f});

		physics->getDynamicsWorld()->addCollisionObject(&m_ghost, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter | btBroadphaseProxy::SensorTrigger
			| btBroadphaseProxy::CharacterFilter);
		physics->getDynamicsWorld()->addAction(&m_characterController);
	}

	void CBullet3Character::setUserData(void* ptr)
	{
		m_ghost.setUserPointer(ptr);
		m_ghost.getCollisionShape()->setUserPointer(ptr);
	}

	CBullet3Character::~CBullet3Character()
	{
		CBullet3Physics* physics = dynamic_cast<CBullet3Physics*>(CPhysics::getPhysicsType());
		ASSERT(physics);
		physics->getDynamicsWorld()->removeAction(&m_characterController);
		physics->getDynamicsWorld()->removeCollisionObject(&m_ghost);
	}

	void CBullet3Character::setWalkDirection(const glm::vec3& dir)
	{
		m_characterController.setWalkDirection({ dir.x, dir.y, dir.z });
	}

	const glm::vec3& CBullet3Character::getWalkDirection() const
	{
		btVector3 vel = m_characterController.getLinearVelocity();
		return { vel.x(), vel.y(), vel.z()};
	}

	void CBullet3Character::jump(const glm::vec3& jump)
	{
		m_characterController.jump({ jump.x, jump.y, jump.z });
	}

	bool CBullet3Character::grounded() const
	{
		return m_characterController.onGround();
	}

	const glm::vec3& CBullet3Character::getLinearVelocity() const
	{
		btVector3 vel = m_characterController.getLinearVelocity();
		return { vel.x(), vel.y(), vel.z() };
	}

	const glm::vec3& CBullet3Character::getAngularVelocity() const
	{
		btVector3 vel = m_characterController.getAngularVelocity();
		return { vel.x(), vel.y(), vel.z() };
	}

	void CBullet3Character::applyImpulse(const glm::vec3& impulse)
	{
		m_characterController.applyImpulse({ impulse.x, impulse.y, impulse.z});
	}

	void CBullet3Character::warp(const glm::vec3& pos)
	{
		m_characterController.warp({ pos.x, pos.y, pos.z });
	}

	const glm::vec3 CBullet3Character::getPosition() const
	{
		btVector3 pos = m_ghost.getWorldTransform().getOrigin();
		return { pos.x(),pos.y(), pos.z()};
	}

	void CBullet3Character::setLinearVelocity(const glm::vec3& dir)
	{
		m_characterController.setLinearVelocity({ dir.x, dir.y, dir.z });
	}


#ifdef DA_REVIEW
	void CBullet3Character::debugDraw()
	{
		CBullet3Physics* physics = dynamic_cast<CBullet3Physics*>(CPhysics::getPhysicsType());
		ASSERT(physics);
		physics->getDynamicsWorld()->debugDrawObject(m_ghost.getWorldTransform(), m_ghost.getCollisionShape(), { 1.f,0.f,0.f });
	}

#endif

}