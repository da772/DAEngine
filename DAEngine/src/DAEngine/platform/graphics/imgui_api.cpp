#include "dapch.h"

#include "imgui_api.h"

#ifdef DA_WINDOW_GLFW

#include <imgui.h>
#include "logger.h"

namespace da::platform
{

	void CImGuiApi::initialize()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO io = ImGui::GetIO();

		ImGui::StyleColorsDark();

		LOG_INFO(ELogChannel::Graphics, "ImGui Initialized v%s", IMGUI_VERSION);
		onInitialize();
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

}

#endif
