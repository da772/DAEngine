#pragma once

#include "../physics.h"
#include <bullet3/btBulletDynamicsCommon.h>

namespace da::physics
{
	class CBullet3DebugDraw;

	class CBullet3Physics : public CPhysicsType
	{
	public:
		void initialize() override;
		void update(float dt) override;
		void shutdown() override;

		inline btDiscreteDynamicsWorld* getDynamicsWorld() const { return m_dynamicsWorld; }

		void rayCast(FRayData& ray) override;

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