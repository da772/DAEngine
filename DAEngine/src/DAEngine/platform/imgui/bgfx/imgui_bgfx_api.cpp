#include "dapch.h"
#include "imgui_bgfx_api.h"

#if defined(DA_GRAPHICS_BGFX) && defined(DA_WINDOW_GLFW)
#include "bgfx_imgui.h"
#include <bx/allocator.h>
#include "DAEngine/core/arg_handler.h"
#include <backends/imgui_impl_glfw.cpp>

#ifdef DA_REVIEW
#include "debug/debug_menu_bar.h"
#endif

namespace da::platform {

	CImGuiBgfxApi::CImGuiBgfxApi(graphics::CGraphicsApi* graphicsApi) : CImGuiApi(graphicsApi)
	{
		
	}

	CImGuiBgfxApi::~CImGuiBgfxApi()
	{
		
	}

	void CImGuiBgfxApi::onInitialize()
	{
		m_window->getEventHandler().registerCallback(da::core::EEventType::InputCursorMove, BIND_EVENT_FN(CImGuiBgfxApi, onCursorMove));
		m_window->getEventHandler().registerCallback(da::core::EEventType::InputMouseButton, BIND_EVENT_FN(CImGuiBgfxApi, onMouseButton));
		imguiCreate();

		ImGui_ImplGlfw_InitForOther((GLFWwindow*)m_window->getNativeWindow(), true);

		m_enableDemo = da::core::CArgHandler::contains(HASHSTR("imguidemo"));

#ifdef DA_REVIEW
		debug::CDebugMenuBar::register_debug(HASHSTR("Renderer"), HASHSTR("ImGuiDemo"), &m_enableDemo, [&] {ImGui::ShowDemoWindow(&m_enableDemo); });
#endif
	}

	void CImGuiBgfxApi::onUpdate()
	{
		ImGui_ImplGlfw_NewFrame();
		imguiBeginFrame((uint32_t)m_mx, (uint32_t)m_my, m_mb, 0, m_window->getWindowData().Width, m_window->getWindowData().Height, 0);
		
#ifndef DA_REVIEW
		if (m_enableDemo) ImGui::ShowDemoWindow(&m_enableDemo);
#endif
	}

	void CImGuiBgfxApi::onLateUpdate()
	{
		imguiEndFrame();
	}

	void CImGuiBgfxApi::onShutdown()
	{
#ifdef DA_REVIEW
		debug::CDebugMenuBar::unregister_debug(HASHSTR("Renderer"), HASHSTR("ImGuiDemo"));
#endif
		m_window->getEventHandler().unregisterCallback(da::core::EEventType::InputCursorMove, BIND_EVENT_FN(CImGuiBgfxApi, onCursorMove));
		m_window->getEventHandler().unregisterCallback(da::core::EEventType::InputMouseButton, BIND_EVENT_FN(CImGuiBgfxApi, onMouseButton));
		imguiDestroy();
	}


	void CImGuiBgfxApi::onCursorMove(const core::CEvent& event)
	{
		core::CInputCursorMoveEvent* ms = (core::CInputCursorMoveEvent*)&event;
		m_mx = ms->getX();
		m_my = ms->getY();
	}

	void CImGuiBgfxApi::onMouseButton(const core::CEvent& event)
	{
		core::CInputMouseButtonEvent* btn = (core::CInputMouseButtonEvent*)&event;
		if (btn->getType() == core::EInputType::PRESSED)
		{
			m_mb = 0x1 << btn->getBtn();
			return;
		}

		m_mb = 0;
	}

}

#endif