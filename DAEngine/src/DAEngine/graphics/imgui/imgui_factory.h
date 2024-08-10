#pragma once

#include "imgui_api.h"

namespace da::factory {

	extern class CImGuiFactory {
	public:
		static da::core::CImGuiApi* Create(graphics::CGraphicsApi* graphicsApi);
	};

}
