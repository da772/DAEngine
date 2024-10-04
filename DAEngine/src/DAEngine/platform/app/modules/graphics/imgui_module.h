#pragma once

#include "platform/app/modules/module.h"
#include "platform/app/modules/graphics/graphics_module.h"

namespace da {
	class CImGuiApi;
}

namespace da
{
	class CImGuiModule : public IModule
	{
	public:
		CImGuiModule(CGraphicsModule* graphicsModule);
		virtual ~CImGuiModule();

		void update() override;
		void lateUpdate() override;
		void initialize() override;
		void shutdown() override;

	private:
		CGraphicsModule* m_GraphicsModule;
		CImGuiApi* m_imGuiApi;

	};
}



