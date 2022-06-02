#pragma once

#include "daengine/modules/module.h"
#include "daengine/modules/window/window_module.h"

namespace da::platform
{
	class CGraphicsApi;
}

namespace da::modules
{
	class CGraphicsModule : public IModule
	{
	public:
		CGraphicsModule(CWindowModule* windowsModule);

		void lateUpdate() override;
		void initalize() override;
		void shutdown() override;

	private:
		CWindowModule* m_Window;
		platform::CGraphicsApi* m_GraphicsApi;


	};
}



