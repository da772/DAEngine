#include "dapch.h"
#include "imgui_bgfx_api.h"

#if defined(DA_GRAPHICS_BGFX) && defined(DA_WINDOW_GLFW)
#include "bgfx_imgui.h"
#include <bx/allocator.h>
#include "DAEngine/core/arg_handler.h"

struct FDAllocator : public bx::AllocatorI
{
	/// Allocates, resizes, or frees memory block.
	///
	/// @remark
	///  - Allocate memory block: _ptr == NULL && size > 0
	///  -   Resize memory block: _ptr != NULL && size > 0
	///  -     Free memory block: _ptr != NULL && size == 0
	///
	/// @param[in] _ptr If _ptr is NULL new block will be allocated. If _ptr is not-NULL, and
	///   _size is not 0, memory block will be resized.
	/// @param[in] _size If _ptr is set, and _size is 0, memory will be freed.
	/// @param[in] _align Alignment.
	/// @param[in] _file Debug file path info.
	/// @param[in] _line Debug file line info.
	///
	virtual void* realloc(
		void* _ptr
		, size_t _size
		, size_t _align
		, const char* _file
		, uint32_t _line
	) override {
		if (_ptr == NULL && _size > 0) {
			return m_allocator.allocate(_size);
		}

		if (_ptr != NULL && _size > 0) {
			return m_allocator.reallocate(_ptr, _size);
		}

		if (_ptr != NULL && _size == 0) {
			m_allocator.deallocate(_ptr);
			return nullptr;
		}

		return nullptr;
	}

	da::memory::CImGuiAllocator m_allocator;
};

namespace da::platform {

	CImGuiBgfxApi::CImGuiBgfxApi(core::CGraphicsApi* graphicsApi) : CImGuiApi(graphicsApi)
	{
		da::memory::CMemoryScope scope(da::memory::EMemoryLayer::ImGui);
		m_allocator = new FDAllocator();
	}

	CImGuiBgfxApi::~CImGuiBgfxApi()
	{
		da::memory::CMemoryScope scope(da::memory::EMemoryLayer::ImGui);
		delete m_allocator;
	}

	void CImGuiBgfxApi::onInitialize()
	{
		m_window->getEventHandler().registerCallback(da::core::EEventType::InputCursorMove, BIND_EVENT_FN(CImGuiBgfxApi, onCursorMove));
		m_window->getEventHandler().registerCallback(da::core::EEventType::InputMouseButton, BIND_EVENT_FN(CImGuiBgfxApi, onMouseButton));
		m_window->getEventHandler().registerCallback(da::core::EEventType::InputKeyboard, BIND_EVENT_FN(CImGuiBgfxApi, onKeyboard));
		m_window->getEventHandler().registerCallback(da::core::EEventType::InputMouseScroll, BIND_EVENT_FN(CImGuiBgfxApi, onMouseScroll));
		imguiCreate();

		m_enableDemo = da::core::CArgHandler::contains("imguidemo");
		m_enableMemory = da::core::CArgHandler::contains("memvis");
	}

	void CImGuiBgfxApi::onUpdate()
	{
		imguiBeginFrame((uint32_t)m_mx, (uint32_t)m_my, m_mb, m_msy, m_window->getWindowData().Width, m_window->getWindowData().Height, m_kb);
		m_msx = 0;
		m_msy = 0;
		m_kb = -1;

		if (m_enableMemory) {
			if (ImGui::Begin("Memory", 0)) {
				ImGui::BeginTable("Table", 2, ImGuiTableFlags_BordersInner);
				ImGui::TableSetupColumn("Name");
				ImGui::TableSetupColumn("Allocation");
				ImGui::TableHeadersRow();

				for (uint8_t i = 0; i < (uint8_t)da::memory::EMemoryLayer::INVALID; i++) {
					ImGui::TableNextColumn();
					ImGui::Text("%s", da::memory::get_memory_layer_name((memory::EMemoryLayer)(i)));
					ImGui::TableNextColumn();
					ImGui::Text("%.2f KB", (double)da::memory::get_memory_layers()[i]/(double)1e3);
				}
				ImGui::EndTable();
				ImGui::Separator();
				ImGui::Text("Total: %.2f KB", (double)da::memory::getMemoryAllocated()/ (double)1e3);
			}
			ImGui::End();
		}
		
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