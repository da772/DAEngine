#include "dapch.h"

#include "imgui_api.h"

#ifdef DA_WINDOW_GLFW

#include <imgui.h>
#include "logger.h"
#include "core/memory/memory.h"

namespace da::core
{
	void* imguiAllocate(size_t sz, void* user_data)
	{
		memory::push_memory_layer(memory::EMemoryLayer::ImGui);
		void* ptr = allocate(sz);
		memory::pop_memory_layer();
		return ptr;
	}

	void imguiDeallocate(void* ptr, void* user_data) {
		memory::push_memory_layer(memory::EMemoryLayer::ImGui);
		deallocate(ptr);
		memory::pop_memory_layer();
	}

	void CImGuiApi::initialize()
	{
		IMGUI_CHECKVERSION();
		ImGui::SetAllocatorFunctions(imguiAllocate, imguiDeallocate, nullptr);

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
		ImGui::DestroyContext();
		LOG_INFO(ELogChannel::Graphics, "ImGui Shutdown v%s", IMGUI_VERSION);
	}

	void CImGuiApi::lateUpdate()
	{
		ImGui::EndFrame();
	}

}

#endif
