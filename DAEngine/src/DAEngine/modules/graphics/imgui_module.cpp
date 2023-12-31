#include "dapch.h"
#include "imgui_module.h"
#include "core/imgui/imgui_factory.h"


namespace da::modules
{

	CImGuiModule::CImGuiModule(CGraphicsModule* graphicsModule) : m_GraphicsModule(graphicsModule)
	{
		m_imGuiApi = core::CImGuiFactory::Create(graphicsModule->getGraphicsApi());
	}


	CImGuiModule::~CImGuiModule()
	{

	}


	void CImGuiModule::update()
	{
		m_imGuiApi->update();
	}

	void CImGuiModule::initialize()
	{
		m_imGuiApi->initialize();
	}

	void CImGuiModule::shutdown()
	{
		m_imGuiApi->shutdown();
		delete m_imGuiApi;
	}

	void CImGuiModule::lateUpdate()
	{
		m_imGuiApi->lateUpdate();
	}

}