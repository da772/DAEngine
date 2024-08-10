#pragma once

#include "app/modules/module.h"
#include "app/modules/graphics/graphics_module.h"

namespace da::core {
	class CImGuiApi;
}

namespace da::modules
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
		core::CImGuiApi* m_imGuiApi;

	};
}



