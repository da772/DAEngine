#pragma once
#include "core/graphics/graphics_api.h"
#include "core/window/window.h"
#include "core/core.h"


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

	protected:
		inline CImGuiApi(CGraphicsApi& graphicsApi) : m_graphicsApi(graphicsApi), m_window(m_graphicsApi.getWindow()) {};
 		CGraphicsApi& m_graphicsApi;
		const CWindow& m_window;
	};


}
