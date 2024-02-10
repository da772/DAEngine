#pragma once

#include "physics.h"
#include <bullet3/btBulletDynamicsCommon.h>

namespace da::physics
{
	class CBullet3Physics : public CPhysicsType
	{
	public:
		void initialize() override;
		void update(float dt) override;
		void shutdown() override;

		inline btDiscreteDynamicsWorld* getDynamicsWorld() const { return m_dynamicsWorld; }
	private:
		btDefaultCollisionConfiguration* m_collisionConfiguration;
		btCollisionDispatcher* m_dispatcher;
		btBroadphaseInterface* m_overlappingPairCache;
		btSequentialImpulseConstraintSolver* m_solver;
		btDiscreteDynamicsWorld* m_dynamicsWorld;

	};
}