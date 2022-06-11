#pragma once
#include "graphics_api.h"
#include "platform/window/window.h"

namespace da::platform {

	class CImGuiApi
	{
	public:

		void initialize();
		void update();
		void shutdown();

	protected:
		virtual void onInitialize() {};
		virtual void onUpdate() {};
		virtual void onShutdown() {};

	protected:
		inline CImGuiApi(const CGraphicsApi& graphicsApi) : m_graphicsApi(graphicsApi), m_window(m_graphicsApi.getWindow()) {};
 		const CGraphicsApi& m_graphicsApi;
		const CWindow& m_window;
	};


}