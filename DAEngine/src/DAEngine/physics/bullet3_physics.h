#pragma once

#include "physics/physics.h"
#include <bullet3/btBulletDynamicsCommon.h>

namespace da
{
	class CBullet3DebugDraw;

	class CBullet3Physics : public CPhysicsType
	{
	public:
		void initialize() override;
		void update(float dt) override;
		void shutdown() override;
		void reset() override;

		inline btDiscreteDynamicsWorld* getDynamicsWorld() const { return m_dynamicsWorld; }
		inline btCollisionWorld* getCollisionWorld() const { return m_collisionWorld; }

		void rayCast(FRayData& ray) override;
		void sweepTest(FSweepData& sweep) override;

	private:
		btDefaultCollisionConfiguration* m_collisionConfiguration;
		btCollisionDispatcher* m_dispatcher;
		btBroadphaseInterface* m_overlappingPairCache;
		btSequentialImpulseConstraintSolver* m_solver;
		btDiscreteDynamicsWorld* m_dynamicsWorld;
		btCollisionWorld* m_collisionWorld;
#if defined(DA_DEBUG) || defined(DA_RELEASE)
		CBullet3DebugDraw* m_debugDraw;
#endif

	};
}