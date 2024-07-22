#include "dapch.h"
#include "collision_component.h"
#include <DAEngine/core/events/event_handler.h>

namespace da::core
{
#ifdef DA_REVIEW
	COMPONENT_CPP_DEBUG(CCollisionComponent)
#else
	COMPONENT_CPP(CCollisionComponent)
#endif
	CCollisionComponent::CCollisionComponent(const da::physics::IPhysicsShape& shape, const glm::mat4& offset, const CGuid& id, da::core::CEntity& parent) 
		: m_guid(id), m_parent(parent), m_transform(offset), m_shape(&shape)
	{
		m_collision = da::physics::ICollisionObject::create(shape, 0);
		m_collision->setTransform(m_parent.getTransform().matrix() * m_transform);
	}

	CCollisionComponent::~CCollisionComponent()
	{
		if (m_collision) {
			delete m_collision;
		}
		
	}

	void CCollisionComponent::onInitialize()
	{
		m_collision->addOverlapCallback(BIND_EVENT_FN(da::core::CCollisionComponent, onOverlap));
	}

	void CCollisionComponent::onUpdate(float dt)
	{
		if (m_collision) m_collision->setTransform(m_parent.getTransform().matrix() * m_transform);
	}

	void CCollisionComponent::onShutdown()
	{
		m_collision->removeOverlapCallback(BIND_EVENT_FN(da::core::CCollisionComponent, onOverlap));
	}

	void CCollisionComponent::onOverlap(const da::physics::FCollisionEventData& data)
	{
		LOG_DEBUG(ELogChannel::Core, "Collision Event: %s, Overlapping: %d", m_parent.getTag().c_str(), data.Overlapping);
		if (da::core::CEntity* ent = (da::core::CEntity*)data.Other) {
			if (data.Overlapping) {
				ent->getTransform().offsetRotation({ 70.f, 0.f ,0.f });
			}
		}
	}


	void CCollisionComponent::enable(bool enable /*= true*/)
	{
		m_collision->enable(enable);
	}

#ifdef DA_REVIEW
	void CCollisionComponent::onDebugRender()
	{
		if (m_collision) m_collision->debugDraw();
	}

#endif


}