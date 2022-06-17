#pragma once
#include "platform/graphics/graphics_api.h"
#include "platform/window/window.h"
#include "platform/platform.h"


namespace da::platform {

	class CImGuiApi
	{
	public:
		void initialize();
		void update();
		void shutdown();
		void lateUpdate();

	protected:
		inline virtual void onInitialize() {};
		inline virtual void onUpdate() {};
		inline virtual void onShutdown() {};

	protected:
		inline CImGuiApi(CGraphicsApi& graphicsApi) : m_graphicsApi(graphicsApi), m_window(m_graphicsApi.getWindow()) {};
 		CGraphicsApi& m_graphicsApi;
		const CWindow& m_window;
	};


}
