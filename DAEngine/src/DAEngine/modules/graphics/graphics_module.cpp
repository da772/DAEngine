#include "dapch.h"
#include "graphics_module.h"
#include "core/graphics/graphics_factory.h"

#include "core/memory/memory.h"

namespace da::modules
{

	CGraphicsModule::CGraphicsModule(const CWindowModule& windowsModule) : m_Window(windowsModule)
	{
		da::memory::push_memory_layer(memory::EMemoryLayer::Graphics);
		m_GraphicsApi = core::CGraphicsFactory::Create(windowsModule.getWindow());
		da::memory::pop_memory_layer();
	}

	CGraphicsModule::~CGraphicsModule()
	{

	}

	void CGraphicsModule::initalize()
	{
		da::memory::push_memory_layer(memory::EMemoryLayer::Graphics);
		m_GraphicsApi->initalize();
		da::memory::pop_memory_layer();
	}

	void CGraphicsModule::lateUpdate()
	{
		da::memory::push_memory_layer(memory::EMemoryLayer::Graphics);
		m_GraphicsApi->update();
		da::memory::pop_memory_layer();
	}

	void CGraphicsModule::shutdown()
	{

	}

	void CGraphicsModule::lateShutdown()
	{
		da::memory::push_memory_layer(memory::EMemoryLayer::Graphics);
		m_GraphicsApi->shutdown();
		delete m_GraphicsApi;
		da::memory::pop_memory_layer();
	}
}