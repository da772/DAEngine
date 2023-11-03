#include "dapch.h"
#include "imgui_bgfx_api.h"

#if defined(DA_GRAPHICS_BGFX) && defined(DA_WINDOW_GLFW)
#include "bgfx_imgui.h"
#include <bx/allocator.h>
#include "DAEngine/core/arg_handler.h"

namespace da::platform {

	CImGuiBgfxApi::CImGuiBgfxApi(core::CGraphicsApi* graphicsApi) : CImGuiApi(graphicsApi)
	{
		
	}

	CImGuiBgfxApi::~CImGuiBgfxApi()
	{
		
	}

	void CImGuiBgfxApi::onInitialize()
	{
		m_window->getEventHandler().registerCallback(da::core::EEventType::InputCursorMove, BIND_EVENT_FN(CImGuiBgfxApi, onCursorMove));
		m_window->getEventHandler().registerCallback(da::core::EEventType::InputMouseButton, BIND_EVENT_FN(CImGuiBgfxApi, onMouseButton));
		m_window->getEventHandler().registerCallback(da::core::EEventType::InputKeyboard, BIND_EVENT_FN(CImGuiBgfxApi, onKeyboard));
		m_window->getEventHandler().registerCallback(da::core::EEventType::InputMouseScroll, BIND_EVENT_FN(CImGuiBgfxApi, onMouseScroll));
		imguiCreate();

		m_enableDemo = da::core::CArgHandler::contains("imguidemo");
	}

	void CImGuiBgfxApi::onUpdate()
	{
		imguiBeginFrame((uint32_t)m_mx, (uint32_t)m_my, m_mb, m_msy, m_window->getWindowData().Width, m_window->getWindowData().Height, m_kb);
		m_msx = 0;
		m_msy = 0;
		m_kb = -1;
		
		if (m_enableDemo) ImGui::ShowDemoWindow(&m_enableDemo);
	}

	void CImGuiBgfxApi::onLateUpdate()
	{
		imguiEndFrame();
	}

	void CImGuiBgfxApi::onShutdown()
	{
		m_window->getEventHandler().unregisterCallback(da::core::EEventType::InputCursorMove, BIND_EVENT_FN(CImGuiBgfxApi, onCursorMove));
		m_window->getEventHandler().unregisterCallback(da::core::EEventType::InputMouseButton, BIND_EVENT_FN(CImGuiBgfxApi, onMouseButton));
		m_window->getEventHandler().unregisterCallback(da::core::EEventType::InputKeyboard, BIND_EVENT_FN(CImGuiBgfxApi, onKeyboard));
		m_window->getEventHandler().unregisterCallback(da::core::EEventType::InputMouseScroll, BIND_EVENT_FN(CImGuiBgfxApi, onMouseScroll));
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

	void CImGuiBgfxApi::onKeyboard(const core::CEvent& event)
	{
		core::CInputKeyboardEvent* btn = (core::CInputKeyboardEvent*)&event;

		if (btn->getType() == core::EInputType::PRESSED)
		{
			m_kb = btn->getBtn();
			return;
		}

		m_kb = -1;
	}

	void CImGuiBgfxApi::onMouseScroll(const core::CEvent& event)
	{
		core::CInputScrollEvent* scrll = (core::CInputScrollEvent*)&event;
		
		m_msx = scrll->getXOffset();
		m_msy = scrll->getYOffset();
	}

}

#endif