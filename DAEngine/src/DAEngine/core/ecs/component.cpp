
#include "component.h"

namespace da {
	CComponent::CComponent(const CGuid& guid, CEntity& parent) : m_guid(guid), m_parent(parent)
	{

	}

	const CHashString& CComponent::getTypeHash() 
	{
		LOG_ASSERT(false, ELogChannel::Core, "Attempting to use component without overriden hashType")
		return CHashString();
	}

	const da::CGuid& CComponent::getId() const
	{
		return m_guid;
	}

}