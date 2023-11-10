#pragma once

#include "component.h"
#include "DAEngine/script/script_class.h"

namespace da::core {

	class CScriptComponent {

		COMPONENT_H(CScriptComponent)

	public:
		CScriptComponent(const std::string& script);

		void onInitialize();
		void onUpdate(float dt);
		void onShutdown();

	private:
		std::string m_script;
		da::script::CScriptClass m_scriptClass;

	};
}