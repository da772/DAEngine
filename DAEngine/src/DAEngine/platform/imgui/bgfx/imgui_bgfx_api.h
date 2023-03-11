#pragma once
#include "core/imgui/imgui_api.h"
#include "platform/platform.h"

#if defined(DA_GRAPHICS_BGFX) && defined(DA_WINDOW_GLFW)

namespace da::platform
{

	class CImGuiBgfxApi : public core::CImGuiApi {
	public:
		CImGuiBgfxApi(core::CGraphicsApi* graphicsApi);
		~CImGuiBgfxApi();

		virtual void onInitialize() override;
		virtual void onUpdate() override;
		virtual void onShutdown() override;
		virtual void onLateUpdate() override;

	private:
		void onCursorMove(const core::CEvent& event);
		void onMouseButton(const core::CEvent& event);
		void onKeyboard(const core::CEvent& event);
		void onMouseScroll(const core::CEvent& event);
	private:
		void* m_allocator;
		float m_mx = 0, m_my = 0;
		double m_msx = 0.0f, m_msy = 0.0f;
		int m_mb = 0, m_kb = 0;
	};

}

#endif