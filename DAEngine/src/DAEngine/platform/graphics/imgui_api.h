#pragma once
#include "graphics_api.h"
#include "platform/window/window.h"
#include "platform.h"

#ifdef DA_WINDOW_GLFW

namespace da::platform {

	class CImGuiApi
	{
	public:
		void initialize();
		void update();
		void shutdown();

	protected:
		inline virtual void onInitialize() {};
		inline virtual void onUpdate() {};
		inline virtual void onShutdown() {};

	protected:
		inline CImGuiApi(const CGraphicsApi& graphicsApi) : m_graphicsApi(graphicsApi), m_window(m_graphicsApi.getWindow()) {};
 		const CGraphicsApi& m_graphicsApi;
		const CWindow& m_window;
	};


}
#endif
