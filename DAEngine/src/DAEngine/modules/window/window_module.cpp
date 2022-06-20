#include "dapch.h"
#include "window_module.h"
#include "core/window/window_factory.h"
#include "core/memory/memory.h"

namespace da::modules
{
	CWindowModule::CWindowModule(const core::FWindowData& windowData)
	{
		da::memory::push_memory_layer(memory::EMemoryLayer::Graphics);
		m_Window = core::CWindowFactory::CreateWindow(windowData);
		da::memory::pop_memory_layer();
	}


	CWindowModule::~CWindowModule()
	{

	}

	void CWindowModule::update()
	{
		da::memory::push_memory_layer(memory::EMemoryLayer::Graphics);
		m_Window->update();
		da::memory::pop_memory_layer();
	}

	void CWindowModule::lateUpdate()
	{

	}

	void CWindowModule::initalize()
	{
		da::memory::push_memory_layer(memory::EMemoryLayer::Graphics);
		m_Window->initalize();
		da::memory::pop_memory_layer();
	}

	void CWindowModule::shutdown()
	{
		
	}

	const da::core::CWindow& CWindowModule::getWindow() const
	{
		return *m_Window;
	}

	void CWindowModule::lateShutdown()
	{
		da::memory::push_memory_layer(memory::EMemoryLayer::Graphics);
		m_Window->shutdown();
		delete m_Window;
		da::memory::pop_memory_layer();
	}

	da::core::events::CEventHandler& CWindowModule::getEventHandler()
	{
		return m_Window->getEventHandler();
	}

}