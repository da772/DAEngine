#pragma once
#include "core.h"
#include "physics_shape.h"
#include "dastd.h"

namespace da
{
	class ICollisionObject;

	struct FCollisionEventData
	{
		ICollisionObject* This;
		void* Other;
		uint32_t Flags;
		bool Overlapping : 1;
	};

	class ICollisionObject
	{
	public:
		static ICollisionObject* create(const IPhysicsShape& shape, uint32_t flags);
		inline virtual ~ICollisionObject() {};

		void addOverlapCallback(const std::function<void(const FCollisionEventData& data)>& func);
		void removeOverlapCallback(const std::function<void(const FCollisionEventData& data)>& func);
		virtual void enable(bool enable);
		virtual bool isEnabled() const;
		virtual void setTransform(const glm::mat4& transform) = 0;

#ifdef DA_REVIEW
		inline virtual void debugDraw() {};
#endif

	protected:
		ICollisionObject(IPhysicsShape* shape, uint32_t flags);
		void invokeOverlapCallback(const FCollisionEventData& data);
		bool m_enabled;
		

	protected:
		IPhysicsShape* m_shape;
		uint32_t m_flags;
		std::vector<std::function<void(const FCollisionEventData& data)>> m_callbacks;
	};
}