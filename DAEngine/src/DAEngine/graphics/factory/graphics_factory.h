#pragma once

#include "graphics/graphics_api.h"
#include "graphics/window/window.h"

using namespace da::graphics;

namespace da::factory
{
	extern class CGraphicsFactory
	{
	public:
		static CGraphicsApi* Create(da::core::CWindow* window);

	};
}
