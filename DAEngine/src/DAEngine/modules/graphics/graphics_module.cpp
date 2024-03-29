#include "dapch.h"
#include "graphics_module.h"
#include "core/graphics/graphics_factory.h"

#include "core/memory/memory.h"

namespace da::modules
{

	CGraphicsModule::CGraphicsModule(const CWindowModule& windowsModule) : m_Window(windowsModule)
	{
		m_GraphicsApi = core::CGraphicsFactory::Create(windowsModule.getWindow());
	}

	CGraphicsModule::~CGraphicsModule()
	{

	}

	void CGraphicsModule::initalize()
	{
		m_GraphicsApi->initalize();
	}

	void CGraphicsModule::lateUpdate()
	{
		m_GraphicsApi->update();
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