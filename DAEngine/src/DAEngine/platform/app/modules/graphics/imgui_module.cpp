
#include "imgui_module.h"
#include "graphics/imgui/imgui_factory.h"


namespace da::modules
{

	CImGuiModule::CImGuiModule(CGraphicsModule* graphicsModule) : m_GraphicsModule(graphicsModule)
	{
		m_imGuiApi = factory::CImGuiFactory::Create(graphicsModule->getGraphicsApi());
	}


	CImGuiModule::~CImGuiModule()
	{

	}


	void CImGuiModule::update()
	{
		PROFILE()
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
		PROFILE()
		m_imGuiApi->lateUpdate();
	}

}