#include "physics.h"

namespace da
{
	da::CPhysicsType* CPhysics::s_physicsType;
	double CPhysics::s_fixedTime = 1.0 / 144.0;
	double CPhysics::s_lastUpdate = 0.f;

	void CPhysics::initialize()
	{
		setPhysics();
		s_physicsType->initialize();
	}

	void CPhysics::update(float dt)
	{
		s_lastUpdate += dt;
		if (s_lastUpdate < s_fixedTime) {
			return;
		}

		int updateCount = std::floor(s_lastUpdate / s_fixedTime);
		s_lastUpdate = std::max(s_lastUpdate-(double)updateCount, 0.0);

		if (updateCount > 10) {
			LOG_WARN(ELogChannel::Physics, "Skipping physics update, count too high: %d", updateCount);
			return;
		}
		ASSERT(s_physicsType);
		for (int i = 0; i < updateCount; i++) {
			PROFILE("Physics Tick")
			s_physicsType->update(s_fixedTime);
		}
	}

	void CPhysics::shutdown()
	{
		ASSERT(s_physicsType);
		s_physicsType->shutdown();
		delete s_physicsType;
	}


	da::CPhysicsType* CPhysics::getPhysicsType()
	{
		return s_physicsType;
	}

	void CPhysics::rayCast(FRayData& ray)
	{
		ASSERT(s_physicsType);
		s_physicsType->rayCast(ray);
	}


	void CPhysics::sweepTest(FSweepData& sweep)
	{
		ASSERT(s_physicsType);
		s_physicsType->sweepTest(sweep);
	}


	double CPhysics::getFixedTime()
	{
		return s_fixedTime;
	}
}