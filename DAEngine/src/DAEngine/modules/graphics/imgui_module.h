#pragma once

#include "daengine/modules/module.h"
#include "daengine/modules/graphics/graphics_module.h"

namespace da::platform {
	class CImGuiApi;
}

namespace da::modules
{
	class CImGuiModule : public IModule
	{
	public:
		CImGuiModule(const CGraphicsModule& graphicsModule);

		void update() override;
		void lateUpdate() override;
		void initalize() override;
		void shutdown() override;

	private:
		const CGraphicsModule& m_GraphicsModule;
		platform::CImGuiApi* m_imGuiApi;

	};
}



