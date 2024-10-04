#pragma once

#include "graphics/window/window.h"

namespace da {

	class CWindowFactory
	{
	public:
		static da::CWindow* CreateWindow(const da::FWindowData& windowData);


	};


}
