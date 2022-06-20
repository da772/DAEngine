#include "dapch.h"
#include "imgui_module.h"
#include "core/imgui/imgui_factory.h"
#include "core/memory/memory.h"

namespace da::modules
{

	CImGuiModule::CImGuiModule(const CGraphicsModule& graphicsModule) : m_GraphicsModule(graphicsModule)
	{
		memory::push_memory_layer(memory::EMemoryLayer::ImGui);
		m_imGuiApi = core::CImGuiFactory::Create(*graphicsModule.getGraphicsApi());
		memory::pop_memory_layer();
	}


	CImGuiModule::~CImGuiModule()
	{

	}


	void CImGuiModule::update()
	{
		memory::push_memory_layer(memory::EMemoryLayer::ImGui);
		m_imGuiApi->update();
		memory::pop_memory_layer();
	}

	void CImGuiModule::initalize()
	{
		memory::push_memory_layer(memory::EMemoryLayer::ImGui);
		m_imGuiApi->initialize();
		memory::pop_memory_layer();
	}

	void CImGuiModule::shutdown()
	{
		memory::push_memory_layer(memory::EMemoryLayer::ImGui);
		m_imGuiApi->shutdown();
		delete m_imGuiApi;
		memory::pop_memory_layer();
	}

	void CImGuiModule::lateUpdate()
	{
		memory::push_memory_layer(memory::EMemoryLayer::ImGui);
		m_imGuiApi->lateUpdate();
		memory::pop_memory_layer();
	}

}