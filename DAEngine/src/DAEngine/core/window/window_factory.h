#pragma once
#include "window.h"

namespace da::core {

	class CWindowFactory
	{
	public:
		static CWindow* CreateWindow(const FWindowData& windowData);


	};


}
