#include "dapch.h"
#include "physics.h"
#include "bullet3_physics.h"

namespace da::physics
{
	da::physics::CPhysicsType* CPhysics::s_physicsType;

	void CPhysics::initialize()
	{
		s_physicsType = new CBullet3Physics();
		s_physicsType->initialize();
	}

	void CPhysics::update(float dt)
	{
		s_physicsType->update(dt);
	}

	void CPhysics::shutdown()
	{
		s_physicsType->shutdown();
		delete s_physicsType;
	}


	da::physics::CPhysicsType* CPhysics::getPhysicsType()
	{
		return s_physicsType;
	}

}