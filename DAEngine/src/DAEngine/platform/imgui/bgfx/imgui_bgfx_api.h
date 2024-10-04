#pragma once
#include "graphics/imgui/imgui_api.h"
#include "platform/platform.h"

#if defined(DA_GRAPHICS_BGFX) && defined(DA_WINDOW_GLFW)

namespace da
{
	class CImGuiBgfxApi : public CImGuiApi {
	public:
		CImGuiBgfxApi(da::CGraphicsApi* graphicsApi);
		~CImGuiBgfxApi();

		virtual void onInitialize() override;
		virtual void onUpdate() override;
		virtual void onShutdown() override;
		virtual void onLateUpdate() override;

	private:
		void onCursorMove(const CEvent& event);
		void onMouseButton(const CEvent& event);
	private:
		float m_mx = 0, m_my = 0;
		int m_mb = 0;
		bool m_enableDemo = false;
	};

}

#endif