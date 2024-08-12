

#include "physics/physics_rigid_body.h"
#include "physics_bullet3_rigid_body.h"

namespace da::physics
{

	IPhysicsRigidBody::IPhysicsRigidBody(IPhysicsShape* shape, IPhysicsMotionState* state, float mass, const glm::vec3& inertia) : m_shape(shape)
	{

	}

	da::physics::IPhysicsShape* IPhysicsRigidBody::getShape() const
	{
		return m_shape;
	}

	IPhysicsRigidBody* IPhysicsRigidBody::create(IPhysicsShape* shape, IPhysicsMotionState* state, float mass, const glm::vec3& inertia)
	{
		return new CBullet3RigidBody(shape, state, mass, inertia);
	}

}