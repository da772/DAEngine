#pragma once
#include "graphics/graphics_api.h"
#include "graphics/window/window.h"
#include "core.h"


namespace ImGui
{
	void extern PushLargeFont();
}

namespace da::core {

	class CImGuiApi
	{
	public:
		inline virtual ~CImGuiApi() {};
		void initialize();
		void update();
		void shutdown();
		void lateUpdate();

	protected:
		inline virtual void onInitialize() {};
		inline virtual void onUpdate() {};
		inline virtual void onShutdown() {};
		inline virtual void onLateUpdate() { };
		virtual void setupStyle();

	protected:
		inline CImGuiApi(graphics::CGraphicsApi* graphicsApi) : m_graphicsApi(graphicsApi), m_window(m_graphicsApi->getWindow()) {};
		graphics::CGraphicsApi* m_graphicsApi;
		CWindow* m_window;
	};


}
