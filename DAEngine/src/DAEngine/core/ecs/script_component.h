#pragma once

#include "component.h"
#include "DAEngine/script/script_class.h"

namespace da::core {

	class CScriptComponent {

		COMPONENT_H(CScriptComponent)

	public:
		CScriptComponent(const std::string& script, const std::string& className, CEntity& parent);

		void onInitialize();
		void onUpdate(float dt);
		void onShutdown();
        
        void reload();

	private:
		std::string m_script;
#ifndef DA_TEST
		da::script::CScriptClass m_scriptClass;
#endif

	};
}
