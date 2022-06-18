#pragma once

#include "graphics_api.h"
#include "core/window/window.h"

namespace da::core
{
	class CGraphicsFactory
	{
	public:
		static CGraphicsApi* Create(const CWindow& window);

	};
}
