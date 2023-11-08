#include "dapch.h"
#include "component.h"

namespace da::core {

	CComponent::CComponent(const CGuid& guid) : m_guid(guid)
	{

	}

	const CHashString& CComponent::getTypeHash() 
	{
		LOG_ASSERT(false, ELogChannel::Core, "Attempting to use component without overriden hashType")
		return CHashString();
	}

	const da::core::CGuid& CComponent::getId() const
	{
		return m_guid;
	}

}