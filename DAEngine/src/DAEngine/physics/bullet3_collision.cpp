

#include "bullet3_collision.h"
#include "bullet3_physics.h"
#include "physics_bullet3_shape.h"
#include <bullet3/btBulletDynamicsCommon.h>
#include <bullet3/BulletCollision/CollisionDispatch/btGhostObject.h>
#include <bullet3/BulletCollision/CollisionDispatch/btCollisionObject.h>
#include <bullet3/Bullet3Common/b3Transform.h>
#include <core/events/event_handler.h>

namespace da
{

	CBullet3Collision::CBullet3Collision(IPhysicsShape* shape, uint32_t flags) : ICollisionObject(shape, flags)
	{
		m_enabled = false;
		CBullet3Physics* physics = dynamic_cast<CBullet3Physics*>(CPhysics::getPhysicsType());
		ASSERT(physics);

		CBullet3Shape* b3Shape = dynamic_cast<CBullet3Shape*>(shape);
		ASSERT(b3Shape);
		btConvexShape* convexShape = dynamic_cast<btConvexShape*>(b3Shape->getShape());
		ASSERT(convexShape);

		m_ghost.setCollisionShape(convexShape);
		m_ghost.setCollisionFlags(btCollisionObject::CF_DYNAMIC_OBJECT | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		m_ghost.Callback = BIND_EVENT_FN_3(CBullet3Collision, onOverlap);

		btTransform startTransform;
		startTransform.setIdentity();
		startTransform.setOrigin(btVector3(0, 0, 0));
		m_ghost.setWorldTransform(startTransform);

		physics->getDynamicsWorld()->addCollisionObject((btCollisionObject*)&m_ghost, btBroadphaseProxy::SensorTrigger, btBroadphaseProxy::AllFilter);
	}

	void CBullet3Collision::setTransform(const glm::mat4& transform)
	{
		btTransform btT;
		btT.setFromOpenGLMatrix(glm::value_ptr(transform));
		m_ghost.setWorldTransform(btT);
	}

#ifdef DA_REVIEW
	void CBullet3Collision::debugDraw()
	{
		CBullet3Physics* physics = dynamic_cast<CBullet3Physics*>(CPhysics::getPhysicsType());
		ASSERT(physics);
		physics->getDynamicsWorld()->debugDrawObject(m_ghost.getWorldTransform(), m_ghost.getCollisionShape(), { 1.f,0.f,1.f });
	}
#endif

	void CBullet3Collision::onOverlap(btBroadphaseProxy* otherProxy, btBroadphaseProxy* thisProxy, bool overlap)
	{
		FCollisionEventData data;
		data.Flags = m_flags;

		
		data.Other = nullptr;
		btRigidBody* rb = (btRigidBody*)otherProxy->m_clientObject;

		if (rb) {
			data.Other = rb->getUserPointer();
		}

		if (!data.Other)
		{
			data.Other = rb->getCollisionShape()->getUserPointer();
		}

		data.This = this;
		data.Overlapping = overlap;
		invokeOverlapCallback(data);
	}

	CBullet3Collision::~CBullet3Collision()
	{
		CBullet3Physics* physics = dynamic_cast<CBullet3Physics*>(CPhysics::getPhysicsType());
		ASSERT(physics);
		physics->getDynamicsWorld()->removeCollisionObject(&m_ghost);
	}

	void CBullet3Collision::enable(bool enable)
	{
		if (enable == m_enabled) return;

		ICollisionObject::enable(enable);

		CBullet3Physics* physics = dynamic_cast<CBullet3Physics*>(CPhysics::getPhysicsType());
		ASSERT(physics);
		
		if (!enable) {
			physics->getDynamicsWorld()->removeCollisionObject(&m_ghost);
			return;
		}
		
		physics->getDynamicsWorld()->addCollisionObject(&m_ghost, btBroadphaseProxy::SensorTrigger, btBroadphaseProxy::AllFilter);
	}

	void CBullet3GhostObject::addOverlappingObjectInternal(btBroadphaseProxy* otherProxy, btBroadphaseProxy* thisProxy /*= 0*/)
	{
		btPairCachingGhostObject::addOverlappingObjectInternal(otherProxy, thisProxy);
		Callback(otherProxy, thisProxy, true);
	}

	void CBullet3GhostObject::removeOverlappingObjectInternal(btBroadphaseProxy* otherProxy, btDispatcher* dispatcher, btBroadphaseProxy* thisProxy /*= 0*/)
	{
		btPairCachingGhostObject::removeOverlappingObjectInternal(otherProxy, dispatcher, thisProxy);
		Callback(otherProxy, thisProxy, false);
	}

}