#pragma once

#include "graphics/graphics_api.h"
#include "graphics/window/window.h"

using namespace da;

namespace da
{
	class CGraphicsFactory
	{
	public:
		static CGraphicsApi* Create(da::CWindow* window);

	};
}
