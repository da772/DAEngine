#pragma once

#include <core/ecs/component.h>
#include <physics/physics_collision.h>

namespace da::core
{
	class CCollisionComponent
	{
#ifdef DA_REVIEW
		COMPONENT_H_DEBUG(CCollisionComponent)
#else
		COMPONENT_H(CCollisionComponent)
#endif
	public:
		CCollisionComponent(const da::physics::IPhysicsShape& shape, const glm::mat4& offset, const CGuid& id, da::core::CEntity& parent);
		virtual ~CCollisionComponent();
		void enable(bool enable = true);
		void setCallback(const std::function<void(const da::physics::FCollisionEventData& data)>& func);

	protected:
		void onInitialize();
		void onUpdate(float dt);
		void onShutdown();

#ifdef DA_REVIEW
		void onDebugRender();

#endif

	private:
		void onOverlap(const da::physics::FCollisionEventData& data);


	private:
		da::physics::ICollisionObject* m_collision;
		const da::physics::IPhysicsShape* m_shape;
		glm::mat4 m_transform;
		bool m_enabled = true;
		std::function<void(const da::physics::FCollisionEventData& data)> m_func;

	};
}