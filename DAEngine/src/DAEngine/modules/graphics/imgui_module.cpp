#include "dapch.h"
#include "imgui_module.h"
#include "core/imgui/imgui_factory.h"

namespace da::modules
{

	CImGuiModule::CImGuiModule(const CGraphicsModule& graphicsModule) : m_GraphicsModule(graphicsModule), m_imGuiApi(core::CImGuiFactory::Create(*graphicsModule.getGraphicsApi()))
	{

	}

	void CImGuiModule::update()
	{
		m_imGuiApi->update();
	}

	void CImGuiModule::initalize()
	{
		m_imGuiApi->initialize();
	}

	void CImGuiModule::shutdown()
	{
		m_imGuiApi->shutdown();
	}

	void CImGuiModule::lateUpdate()
	{
		m_imGuiApi->lateUpdate();
	}

}