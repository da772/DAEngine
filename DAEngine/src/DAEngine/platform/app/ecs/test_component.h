#pragma once

#include "core/ecs/component.h"
#include <functional>

namespace da
{
	class CTestComponent  {
		COMPONENT_H(CTestComponent);

	public:
		CTestComponent(const char* data1, const char* data2, CEntity& parent);

		void onInitialize() ;
		void onUpdate(float dt);
		void onShutdown();

		std::string data1, data2;

	};
}