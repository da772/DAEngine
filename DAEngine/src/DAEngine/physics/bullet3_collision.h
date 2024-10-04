#pragma once
#include "core.h"
#include "physics/physics_collision.h"
#include <bullet3/BulletCollision/CollisionDispatch/btGhostObject.h>
#include <bullet3/BulletCollision/CollisionDispatch/btCollisionObject.h>

namespace da
{
	class CBullet3GhostObject : public btPairCachingGhostObject
	{
	public:
		virtual void addOverlappingObjectInternal(btBroadphaseProxy* otherProxy, btBroadphaseProxy* thisProxy = 0) override;
		virtual void removeOverlappingObjectInternal(btBroadphaseProxy* otherProxy, btDispatcher* dispatcher, btBroadphaseProxy* thisProxy = 0) override;

		std::function<void(btBroadphaseProxy* otherProxy, btBroadphaseProxy* thisProxy, bool overlap)> Callback;
	};

	class CBullet3Collision : public ICollisionObject
	{
	public:
		CBullet3Collision(IPhysicsShape* shape, uint32_t flags);
		virtual ~CBullet3Collision();

		virtual void setTransform(const glm::mat4& transform) override;
		virtual void enable(bool enable) override;

#ifdef DA_REVIEW
		virtual void debugDraw() override;
#endif
	private:
		void onOverlap(btBroadphaseProxy* otherProxy, btBroadphaseProxy* thisProxy, bool overlap);

	private:
		CBullet3GhostObject m_ghost;
		btConvexShape* m_shape;
	};
}
