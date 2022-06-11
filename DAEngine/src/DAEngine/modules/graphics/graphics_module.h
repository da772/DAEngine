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
		CGraphicsModule(const CWindowModule& windowsModule);

		void lateUpdate() override;
		void initalize() override;
		void shutdown() override;

		inline da::platform::CGraphicsApi* getGraphicsApi() const {
			return m_GraphicsApi;
		}

	private:
		const CWindowModule& m_Window;
		platform::CGraphicsApi* m_GraphicsApi;


	};
}



