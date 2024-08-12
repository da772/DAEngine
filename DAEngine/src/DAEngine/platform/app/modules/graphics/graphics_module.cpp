
#include "graphics_module.h"
#include "graphics/factory/graphics_factory.h"



namespace da::modules
{

	CGraphicsModule::CGraphicsModule(CWindowModule* windowsModule) : m_Window(windowsModule)
	{
		m_GraphicsApi = factory::CGraphicsFactory::Create(windowsModule->getWindow());
	}

	CGraphicsModule::~CGraphicsModule()
	{

	}

	void CGraphicsModule::initialize()
	{
		m_GraphicsApi->initialize();
	}

	void CGraphicsModule::update()
	{
		PROFILE()
		m_GraphicsApi->update();
	}

	void CGraphicsModule::lateUpdate()
	{
		PROFILE()
		m_GraphicsApi->lateUpdate();
	}

	void CGraphicsModule::shutdown()
	{

	}

	void CGraphicsModule::lateShutdown()
	{
		m_GraphicsApi->shutdown();
		delete m_GraphicsApi;
	}



}