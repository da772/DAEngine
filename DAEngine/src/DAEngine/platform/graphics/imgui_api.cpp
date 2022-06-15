#include "dapch.h"

#include "imgui_api.h"

#ifdef DA_WINDOW_GLFW

#include <imgui.h>

namespace da::platform
{

	void CImGuiApi::initialize()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO io = ImGui::GetIO();

		ImGui::StyleColorsDark();

		onInitialize();
	}

	void CImGuiApi::update()
	{
		onUpdate();
	}

	void CImGuiApi::shutdown()
	{
		onShutdown();
	}

}

#endif
