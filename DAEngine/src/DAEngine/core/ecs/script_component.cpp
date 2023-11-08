#include "dapch.h"
#include "script_component.h"

namespace da::core {

	COMPONENT_CPP(CScriptComponent)

	CScriptComponent::CScriptComponent(const CGuid& guid, const std::string& script) : m_guid(guid), m_script(script)
	{

	}

	void CScriptComponent::onInitialize()
	{

	}

	void CScriptComponent::onUpdate(float dt)
	{

	}

	void CScriptComponent::onShutdown()
	{

	}

}