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
		if (!m_enabled)
		{	
			return;
		}

		if (da::core::CEntity* ent = (da::core::CEntity*)data.Other) {
			if (data.Overlapping && ent != &m_parent) {
				m_func(data);
			}
		}
	}


	void CCollisionComponent::enable(bool enable /*= true*/)
	{
		bool processOverlaps = !m_enabled && enable;
		m_enabled = enable;
	}


	void CCollisionComponent::setCallback(const std::function<void(const da::physics::FCollisionEventData& data)>& func)
	{
		m_func = func;
	}


#ifdef DA_REVIEW
	void CCollisionComponent::onDebugRender()
	{
		if (m_collision) m_collision->debugDraw();
	}

#endif


}