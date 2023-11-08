#pragma once

#include "component.h"

namespace da::core {

	class CScriptComponent {

		COMPONENT_H(CScriptComponent)

	public:
		CScriptComponent(const CGuid& guid, const std::string& script);

		void onInitialize();
		void onUpdate(float dt);
		void onShutdown();

	private:
		std::string m_script;

	};
}