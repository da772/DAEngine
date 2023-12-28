#include "dapch.h"
#include "script_component.h"
#include "daengine/script/script_engine.h"
#include <lua/lua.h>

#ifdef DA_DEBUG
#include <imgui.h>
#endif

namespace da::core {

#ifdef DA_DEBUG
	COMPONENT_CPP_DEBUG(CScriptComponent)
#else
	COMPONENT_CPP(CScriptComponent)
#endif

	CScriptComponent::CScriptComponent(const std::string& script, const std::string& className, CEntity& parent) : m_guid(CGuid::Generate()), m_parent(parent), m_script(script)
	{
        m_scriptClass = da::script::CScriptClass(m_script, className, "component");
	}

	void CScriptComponent::onInitialize()
	{
		m_scriptClass.setup(m_parent.getId(), m_guid);

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
        m_scriptClass.setup(m_parent.getId(), m_guid);
    }

#ifdef DA_DEBUG
	void CScriptComponent::onDebugRender() {

		ImGui::Text("Script: %s", m_script.c_str());
		ImGui::Text("Class: %s", m_scriptClass.getClass().c_str());
		ImGui::Text("Obj: %s", m_scriptClass.getObjName().c_str());

	}
#endif



}
