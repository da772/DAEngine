#pragma once

#include "core/ecs/component.h"
#include "script/script_class.h"

namespace da {

	class CScriptComponent {

#ifdef DA_REVIEW
		COMPONENT_H_DEBUG(CScriptComponent)
#else
		COMPONENT_H(CScriptComponent)
#endif

	public:
		CScriptComponent(const std::string& script, const std::string& className, CEntity& parent);

		void onInitialize();
		void onUpdate(float dt);
		void onShutdown();
        
        void reload(bool soft);

#ifdef DA_REVIEW
		void onDebugRender();
#endif

	private:
		std::string m_script;
		da::CScriptClass m_scriptClass;

	};
}
