#include "dapch.h"
#include "graphics_module.h"
#include "core/graphics/graphics_factory.h"



namespace da::modules
{

	CGraphicsModule::CGraphicsModule(CWindowModule* windowsModule) : m_Window(windowsModule)
	{
		m_GraphicsApi = graphics::CGraphicsFactory::Create(windowsModule->getWindow());
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
		m_GraphicsApi->update();
	}

	void CGraphicsModule::lateUpdate()
	{
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