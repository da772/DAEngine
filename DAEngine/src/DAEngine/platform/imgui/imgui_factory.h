#pragma once

#include "imgui_api.h"

namespace da::platform {

	class CImGuiFactory {
	public:
		static CImGuiApi* Create(CGraphicsApi& graphicsApi);
	};

}
