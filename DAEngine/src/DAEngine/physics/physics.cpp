#include "dapch.h"
#include "physics.h"
#include "bullet3/bullet3_physics.h"

namespace da::physics
{
	da::physics::CPhysicsType* CPhysics::s_physicsType;
	double CPhysics::s_fixedTime = 1.0 / 60.0;
	double CPhysics::s_lastUpdate = 0.f;

	void CPhysics::initialize()
	{
		s_physicsType = new CBullet3Physics();
		s_physicsType->initialize();
	}

	void CPhysics::update(float dt)
	{
		s_lastUpdate += dt;
		if (s_lastUpdate < s_fixedTime) {
			return;
		}

		int updateCount = std::floor(s_lastUpdate / s_fixedTime);
		s_lastUpdate = 0.0;

		ASSERT(s_physicsType);
		for (int i = 0; i < updateCount; i++) {
			s_physicsType->update(s_fixedTime);
		}
	}

	void CPhysics::shutdown()
	{
		ASSERT(s_physicsType);
		s_physicsType->shutdown();
		delete s_physicsType;
	}


	da::physics::CPhysicsType* CPhysics::getPhysicsType()
	{
		return s_physicsType;
	}

	void CPhysics::rayCast(FRayData& ray)
	{
		ASSERT(s_physicsType);
		s_physicsType->rayCast(ray);
	}

	double CPhysics::getFixedTime()
	{
		return s_fixedTime;
	}

}