#pragma once

#include "imgui_api.h"

namespace da {

	class CImGuiFactory {
	public:
		static da::CImGuiApi* Create(da::CGraphicsApi* graphicsApi);
	};

}
