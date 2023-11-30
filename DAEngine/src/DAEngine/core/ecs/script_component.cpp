#include "dapch.h"
#include "script_component.h"
#include "daengine/script/script_engine.h"

namespace da::core {

	COMPONENT_CPP(CScriptComponent)

	CScriptComponent::CScriptComponent(const std::string& script, const std::string& className, CEntity& parent) : m_guid(CGuid::Generate()), m_parent(parent), m_script(script)
	{
#ifndef DA_TEST
        m_scriptClass = da::script::CScriptClass(m_script, className, "component");
#endif
	}

	void CScriptComponent::onInitialize()
	{
#ifndef DA_TEST
		m_scriptClass.setup();

		m_scriptClass.classInitialize();
#endif

	}

	void CScriptComponent::onUpdate(float dt)
	{
#ifndef DA_TEST
		m_scriptClass.classUpdate(dt);
#endif
	}

	void CScriptComponent::onShutdown()
	{
#ifndef DA_TEST
		m_scriptClass.classShutdown();

		m_scriptClass.cleanup(false);
#endif
	}

    void CScriptComponent::reload() {
        
#ifndef DA_TEST
        m_scriptClass.cleanup(true);
        m_scriptClass.setup();
#endif
    }

}
