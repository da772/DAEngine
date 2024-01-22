#pragma once

#include "imgui_api.h"

namespace da::core {

	class CImGuiFactory {
	public:
		static CImGuiApi* Create(graphics::CGraphicsApi* graphicsApi);
	};

}
