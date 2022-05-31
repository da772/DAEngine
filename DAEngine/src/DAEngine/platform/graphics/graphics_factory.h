#pragma once

#include "graphics_api.h"
#include "platform/window/window.h"

namespace da::platform
{
	namespace graphics {

		class CGraphicsFactory
		{
		public:
			static CGraphicsApi* Create(const CWindow& window);

		};
	}

}
