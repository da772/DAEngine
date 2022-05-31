#pragma once
#include "window.h"

namespace da::platform::window {

	class CWindowFactory
	{
	public:
		static CWindow* CreateWindow(const FWindowData& windowData);


	};


}
