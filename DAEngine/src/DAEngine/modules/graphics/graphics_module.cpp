#include "dapch.h"
#include "graphics_module.h"
#include "core/graphics/graphics_factory.h"

namespace da::modules
{

	CGraphicsModule::CGraphicsModule(const CWindowModule& windowsModule) : m_Window(windowsModule), m_GraphicsApi(core::CGraphicsFactory::Create(windowsModule.getWindow()))
	{

	}

	void CGraphicsModule::lateUpdate()
	{
		m_GraphicsApi->update();
	}

	void CGraphicsModule::initalize()
	{
		m_GraphicsApi->initalize();
	}

	void CGraphicsModule::shutdown()
	{

	}

	void CGraphicsModule::lateShutdown()
	{
		m_GraphicsApi->shutdown();
	}

}