#pragma once

#include <core/ecs/component.h>
#include <physics/physics_collision.h>

namespace da
{
	class CCollisionComponent
	{
#ifdef DA_REVIEW
		COMPONENT_H_DEBUG(CCollisionComponent)
#else
		COMPONENT_H(CCollisionComponent)
#endif
	public:
		CCollisionComponent(const da::IPhysicsShape& shape, const glm::mat4& offset, const CGuid& id, da::CEntity& parent);
		virtual ~CCollisionComponent();
		void enable(bool enable = true);
		void setCallback(const std::function<void(const da::FCollisionEventData& data)>& func);

	protected:
		void onInitialize();
		void onUpdate(float dt);
		void onShutdown();

#ifdef DA_REVIEW
		void onDebugRender();

#endif

	private:
		void onOverlap(const da::FCollisionEventData& data);


	private:
		da::ICollisionObject* m_collision;
		const da::IPhysicsShape* m_shape;
		glm::mat4 m_transform;
		bool m_enabled = true;
		std::function<void(const da::FCollisionEventData& data)> m_func;

	};
}