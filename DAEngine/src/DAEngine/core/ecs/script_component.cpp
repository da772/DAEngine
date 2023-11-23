#include "dapch.h"
#include "script_component.h"
#include "daengine/script/script_engine.h"
#include <lua/lua.h>

namespace da::core {

	COMPONENT_CPP(CScriptComponent)

	CScriptComponent::CScriptComponent(const std::string& script, const std::string& className, CEntity& parent) : m_guid(CGuid::Generate()), m_parent(parent), m_script(script)
	{
        m_scriptClass = da::script::CScriptClass(m_script, className, "component");
	}

	void CScriptComponent::onInitialize()
	{
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

		m_scriptClass.cleanup(false);
	}

    void CScriptComponent::reload() {
        
        m_scriptClass.cleanup(true);
        m_scriptClass.setup();
    }

}
