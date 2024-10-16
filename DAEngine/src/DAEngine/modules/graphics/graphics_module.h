#pragma once

#include "daengine/modules/module.h"
#include "daengine/modules/window/window_module.h"

namespace da::core
{
	class CGraphicsApi;
}

namespace da::modules
{
	class CGraphicsModule : public IModule
	{
	public:
		CGraphicsModule(const CWindowModule& windowsModule);
		virtual ~CGraphicsModule();
		void lateUpdate() override;
		void initalize() override;
		void shutdown() override;
		void lateShutdown() override;

		inline da::core::CGraphicsApi* getGraphicsApi() const {
			return m_GraphicsApi;
		}

	private:
		const CWindowModule& m_Window;
		core::CGraphicsApi* m_GraphicsApi;


	};
}



