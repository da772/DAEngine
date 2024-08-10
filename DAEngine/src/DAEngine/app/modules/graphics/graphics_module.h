#pragma once

#include "app/modules/module.h"
#include "app/modules/window/window_module.h"

namespace da::graphics
{
	class CGraphicsApi;
}

namespace da::modules
{
	class CGraphicsModule : public IModule
	{
	public:
		CGraphicsModule(CWindowModule* windowsModule);
		virtual ~CGraphicsModule();
		void lateUpdate() override;
		void initialize() override;
		void shutdown() override;
		void lateShutdown() override;
		void update() override;

		inline da::graphics::CGraphicsApi* getGraphicsApi() const {
			return m_GraphicsApi;
		}

	private:
		CWindowModule* m_Window;
		graphics::CGraphicsApi* m_GraphicsApi;


	};
}



