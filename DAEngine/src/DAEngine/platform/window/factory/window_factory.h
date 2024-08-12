#pragma once

#include "graphics/window/window.h"

namespace da::factory {

	class CWindowFactory
	{
	public:
		static da::core::CWindow* CreateWindow(const da::core::FWindowData& windowData);


	};


}
