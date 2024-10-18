

#include "imgui_api.h"

#ifdef DA_WINDOW_GLFW

#include <imgui.h>
#include "core/logger.h"

namespace da
{
	void* imguiAllocate(size_t sz, void* user_data)
	{
		void* ptr = malloc(sz);
		return ptr;
	}

	void imguiDeallocate(void* ptr, void* user_data) {
		free(ptr);
	}

	void CImGuiApi::initialize()
	{
		IMGUI_CHECKVERSION();	
		onInitialize();
		LOG_INFO(ELogChannel::Graphics, "ImGui Initialized v%s", IMGUI_VERSION);
		setupStyle();
	}

	void CImGuiApi::update()
	{
		onUpdate();
	}

	void CImGuiApi::shutdown()
	{
		onShutdown();
		LOG_INFO(ELogChannel::Graphics, "ImGui Shutdown v%s", IMGUI_VERSION);
	}

	void CImGuiApi::lateUpdate()
	{
		onLateUpdate();
	}

	void CImGuiApi::setupStyle()
	{
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4* colors = style.Colors;

		// Colors
		colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);          // Background color
		colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);           // Frame background
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);    // Frame hover
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);     // Frame active
		colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);           // Title background
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);     // Title active
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);  // Title collapsed
		colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);            // Button color
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);     // Button hover
		colors[ImGuiCol_ButtonActive] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);      // Button active
		colors[ImGuiCol_Border] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);            // Borders
		colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 0.75f, 0.50f, 1.00f);         // Checkbox checkmark color
		colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 0.75f, 0.50f, 1.00f);        // Slider grab
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 0.85f, 0.60f, 1.00f);  // Slider grab active
		colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);              // Text color
		colors[ImGuiCol_TextDisabled] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);      // Disabled text

		// Style settings
		style.WindowRounding = 4.0f;      // Window rounding
		style.FrameRounding = 3.0f;       // Frame rounding
		style.GrabRounding = 2.0f;        // Slider grab rounding
		style.ScrollbarRounding = 5.0f;   // Scrollbar rounding
		style.ChildRounding = 4.0f;       // Child window rounding
		style.PopupRounding = 4.0f;       // Popup rounding
		style.FramePadding = ImVec2(6, 4); // Frame padding
		style.ItemSpacing = ImVec2(8, 6);  // Spacing between items
		style.WindowPadding = ImVec2(8, 8); // Padding inside windows
		style.IndentSpacing = 20.0f;       // Indentation spacing
		style.ScrollbarSize = 15.0f;       // Scrollbar size

	}

}

#endif
