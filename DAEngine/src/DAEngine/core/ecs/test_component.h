#pragma once

#include "component.h"
#include <functional>

namespace da::core
{
	class CTestComponent  {
		COMPONENT_H(CTestComponent);

	public:
		CTestComponent(const char* data1, const char* data2);

		void onInitialize() ;
		void onUpdate(float dt);
		void onShutdown();

		std::string data1, data2;

	};
}