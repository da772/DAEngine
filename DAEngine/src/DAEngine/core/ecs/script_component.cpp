#include "dapch.h"
#include "script_component.h"
#include "daengine/script/script_engine.h"
#include <lua/lua.h>

namespace da::core {

	COMPONENT_CPP(CScriptComponent)

	CScriptComponent::CScriptComponent(const std::string& script) : m_guid(CGuid::Generate()), m_script(script)
	{

	}

	void CScriptComponent::onInitialize()
	{
		m_scriptClass = da::script::CScriptClass(m_script, "MyComponent", "component");
		
		m_scriptClass.setup();

		m_scriptClass.classInitialize();

	}

	void CScriptComponent::onUpdate(float dt)
	{
		m_scriptClass.classUpdate(dt);
	}

	void CScriptComponent::onShutdown()
	{
		m_scriptClass.classShutdown();

		m_scriptClass.cleanup();
	}

}