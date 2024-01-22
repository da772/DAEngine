#pragma once

#include "graphics_api.h"
#include "core/window/window.h"

namespace da::graphics
{
	class CGraphicsFactory
	{
	public:
		static CGraphicsApi* Create(da::core::CWindow* window);

	};
}
