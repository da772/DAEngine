#include "dapch.h"
#include "physics_collision.h"

#include "bullet3/bullet3_collision.h"

namespace da::physics {

	ICollisionObject::ICollisionObject(IPhysicsShape* shape, uint32_t flags) : m_shape(shape), m_flags(flags), m_enabled(true)
	{
		
	}

	ICollisionObject* ICollisionObject::create(const IPhysicsShape& shape, uint32_t flags)
	{
		return new CBullet3Collision((IPhysicsShape*)&shape, flags);
	}

	void ICollisionObject::addOverlapCallback(const std::function<void(const FCollisionEventData& data)>& func)
	{
		m_callbacks.push_back(func);
	}

	void ICollisionObject::removeOverlapCallback(const std::function<void(const FCollisionEventData& data)>& func)
	{
		for (std::vector<std::function<void(const FCollisionEventData&)>>::const_iterator it = m_callbacks.begin(); it != m_callbacks.end(); it++) {
			if (it->target_type() == func.target_type())
			{
				if (it->target<void(const FCollisionEventData&)>() == func.target<void(const FCollisionEventData&)>()) {
					m_callbacks.erase(it);
					return;
				}
			}
		}
	}

	void ICollisionObject::invokeOverlapCallback(const FCollisionEventData& data)
	{
		for (const std::function<void(FCollisionEventData)>& f : m_callbacks) {
			f(data);
		}
	}

	bool ICollisionObject::isEnabled() const
	{
		return m_enabled;
	}

	void ICollisionObject::enable(bool enable)
	{
		m_enabled = enable;
	}

}