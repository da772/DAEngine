#pragma once

#include "daengine/modules/module.h"
#include "daengine/modules/graphics/graphics_module.h"

namespace da::core {
	class CImGuiApi;
}

namespace da::modules
{
	class CImGuiModule : public IModule
	{
	public:
		CImGuiModule(const CGraphicsModule& graphicsModule);
		virtual ~CImGuiModule();

		void update() override;
		void lateUpdate() override;
		void initalize() override;
		void shutdown() override;

	private:
		const CGraphicsModule& m_GraphicsModule;
		core::CImGuiApi* m_imGuiApi;

	};
}



