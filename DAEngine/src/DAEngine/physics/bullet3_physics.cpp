
#include "bullet3_physics.h"
#include <bullet3/BulletCollision/CollisionDispatch/btGhostObject.h>

#include "debug/graphics_debug_render.h"
#include <glm/gtx/matrix_decompose.hpp>
#include <imgui.h>
#include "core/ecs/entity.h"

#if defined(DA_DEBUG) || defined(DA_RELEASE)
#include "bullet3_debug_draw.h"
#endif
#include "physics_bullet3_shape.h"

namespace da
{
	void CPhysics::setPhysics()
	{
		s_physicsType = new CBullet3Physics();
	}
	
	void CBullet3Physics::initialize()
	{
		m_collisionConfiguration = new btDefaultCollisionConfiguration();
		m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
		m_solver = new btSequentialImpulseConstraintSolver();
		m_solver->setRandSeed(123);
		btVector3 worldMin(-1000, -1000, -1000);
		btVector3 worldMax(1000, 1000, 1000);
		btAxisSweep3* sweepBP = new btAxisSweep3(worldMin, worldMax);
		m_overlappingPairCache = sweepBP;

		m_overlappingPairCache->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());

		m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_overlappingPairCache, m_solver, m_collisionConfiguration);

		m_dynamicsWorld->setGravity(btVector3(0, 0, -10));

		m_collisionWorld = new btCollisionWorld(m_dispatcher, m_overlappingPairCache, m_collisionConfiguration);

#if defined(DA_DEBUG) || defined(DA_RELEASE)
		m_debugDraw = new CBullet3DebugDraw();
		m_dynamicsWorld->setDebugDrawer(m_debugDraw);
		m_collisionWorld->setDebugDrawer(m_debugDraw);
#endif
	}

	void CBullet3Physics::update(float dt)
	{
		PROFILE()
		{
			PROFILE("CBullet3Physics::update::collisionloop")
			for (int j = m_dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
			{
				btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[j];
				btRigidBody* body = btRigidBody::upcast(obj);

				if (body && body->getMotionState())
				{
					if (da::CEntity* e = (da::CEntity*)body->getUserPointer()) {
						if (e->getTransform().isDirty()) {
							e->getTransform().matrix();
						}
					}
				}
			}
		}
		
		{
			PROFILE("CBullet3Physics::update::stepSimulation")
			m_dynamicsWorld->stepSimulation(dt, 10, da::CPhysics::getFixedTime());
		}
		//m_dynamicsWorld->debugDrawWorld();
	}

	void CBullet3Physics::shutdown()
	{
		for (int i = m_dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
		{
			btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
			btRigidBody* body = btRigidBody::upcast(obj);
			if (body && body->getMotionState())
			{
				delete body->getMotionState();
			}
			m_dynamicsWorld->removeCollisionObject(obj);
			delete obj;
		}

		//delete dynamics world
		delete m_dynamicsWorld;

		//delete solver
		delete m_solver;

		//delete broadphase
		delete m_overlappingPairCache;

		//delete dispatcher
		delete m_dispatcher;

		delete m_collisionConfiguration;

		delete m_collisionWorld;


#if defined(DA_DEBUG) || defined(DA_RELEASE)
		delete m_debugDraw;
#endif
	}

	void CBullet3Physics::rayCast(FRayData& ray)
	{
		PROFILE("")
		if (ray.eType == ERayType::All)
		{
			btCollisionWorld::AllHitsRayResultCallback callback({ ray.endPos.x, ray.endPos.y, ray.endPos.z }, { ray.startPos.x, ray.startPos.y, ray.startPos.z });
			m_collisionWorld->rayTest(callback.m_rayFromWorld, callback.m_rayToWorld, callback);
			
			ray.bHit = callback.hasHit();

			if (ray.bHit)
			{
				for (int i = 0; i < callback.m_collisionObjects.size(); i++) {
					FHitData hitData;
					hitData.pEntity = (da::CEntity*)callback.m_collisionObjects[i]->getUserPointer();

					std::vector<FHitData>::iterator it = std::find_if(ray.vHits.begin(), ray.vHits.end(), [hitData](const FHitData& d) {
						return d.pEntity == hitData.pEntity;
					});

					if (it != ray.vHits.end()) {
						continue;
					}

					btVector3 hitPos = callback.m_hitPointWorld[i];
					hitData.position = { hitPos.x(), hitPos.y(), hitPos.z() };
					btVector3 hitNormal = callback.m_hitNormalWorld[i];
					hitData.normal = { hitNormal.x(), hitNormal.y(), hitNormal.z() };

					ray.vHits.push_back(std::move(hitData));
				}
			}
			return;
		}

		btCollisionWorld::ClosestRayResultCallback callback({ ray.endPos.x, ray.endPos.y, ray.endPos.z }, { ray.startPos.x, ray.startPos.y, ray.startPos.z });
		m_collisionWorld->rayTest(callback.m_rayFromWorld, callback.m_rayToWorld, callback);

		ray.bHit = callback.hasHit();

		if (ray.bHit)
		{
			FHitData hitData;
			hitData.pEntity = (da::CEntity*)callback.m_collisionObject->getUserPointer();
			btVector3 hitPos = callback.m_hitPointWorld;
			hitData.position = { hitPos.x(), hitPos.y(), hitPos.z() };
			btVector3 hitNormal = callback.m_hitNormalWorld;
			hitData.normal = { hitNormal.x(), hitNormal.y(), hitNormal.z() };

			ray.vHits.push_back(std::move(hitData));

		}
	}

	void CBullet3Physics::sweepTest(FSweepData& sweep)
	{
		btVector3 startPos(sweep.StartTransform[3].x, sweep.StartTransform[3].y, sweep.StartTransform[3].z);
		btVector3 endPos(sweep.EndTransform[3].x, sweep.EndTransform[3].y, sweep.EndTransform[3].z);
		btCollisionWorld::ClosestConvexResultCallback result(startPos, endPos);
		result.m_collisionFilterMask = btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter;

		CBullet3Shape* shape = dynamic_cast<CBullet3Shape*>(sweep.Shape);
		ASSERT(shape);
		btConvexShape* convShape = dynamic_cast<btConvexShape*>(shape->getShape());
		ASSERT(convShape);

		btTransform startTransform, endTransform;
		startTransform.setFromOpenGLMatrix(glm::value_ptr(sweep.StartTransform));
		endTransform.setFromOpenGLMatrix(glm::value_ptr(sweep.EndTransform));

		m_collisionWorld->convexSweepTest(convShape, startTransform, endTransform, result);

		sweep.bHit = result.hasHit();
		if (sweep.bHit)
		{
			sweep.Hit.position = { result.m_hitPointWorld.x(), result.m_hitPointWorld.y(), result.m_hitPointWorld.z() };
			sweep.Hit.normal = { result.m_hitNormalWorld.x(), result.m_hitNormalWorld.y(), result.m_hitNormalWorld.z() };
			sweep.Hit.pEntity = result.m_hitCollisionObject ? (da::CEntity*)result.m_hitCollisionObject->getUserPointer() : nullptr;
		}
	}


	void CBullet3Physics::reset()
	{
		ASSERT(m_solver);
		m_solver->reset();
	}


}