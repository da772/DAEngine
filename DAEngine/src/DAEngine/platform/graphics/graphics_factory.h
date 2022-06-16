#pragma once

#include "graphics_api.h"
#include "platform/window/window.h"

namespace da::platform
{
	class CGraphicsFactory
	{
	public:
		static CGraphicsApi* Create(const CWindow& window);

	};
}
