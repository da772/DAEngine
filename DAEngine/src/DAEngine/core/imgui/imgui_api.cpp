#include "dapch.h"

#include "imgui_api.h"

#ifdef DA_WINDOW_GLFW

#include <imgui.h>
#include "logger.h"

namespace da::core
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
		//ImGui::SetAllocatorFunctions(imguiAllocate, imguiDeallocate, nullptr);
		/*
		ImGui::CreateContext();
		ImGuiIO io = ImGui::GetIO();

		
		*/

		
		onInitialize();
		LOG_INFO(ELogChannel::Graphics, "ImGui Initialized v%s", IMGUI_VERSION);
		ImGui::StyleColorsDark();
		//ImGui::SetAllocatorFunctions(imguiAllocate, imguiDeallocate, nullptr);
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

}

#endif
