#pragma once
#include "window.h"

namespace da::platform {

	class CWindowFactory
	{
	public:
		static CWindow* CreateWindow(const FWindowData& windowData);


	};


}
