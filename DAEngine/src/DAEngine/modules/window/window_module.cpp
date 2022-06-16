#include "dapch.h"
#include "window_module.h"
#include "platform/window/window_factory.h"

namespace da::modules
{
	CWindowModule::CWindowModule(const platform::FWindowData& windowData) : m_Window(platform::CWindowFactory::CreateWindow(windowData))
	{

	}

	void CWindowModule::update()
	{
		m_Window->update();
	}

	void CWindowModule::lateUpdate()
	{

	}

	void CWindowModule::initalize()
	{
		m_Window->initalize();
	}

	void CWindowModule::shutdown()
	{
		
	}

	const da::platform::CWindow& CWindowModule::getWindow() const
	{
		return *m_Window;
	}

	void CWindowModule::lateShutdown()
	{
		m_Window->shutdown();
	}

	da::core::events::CEventHandler& CWindowModule::getEventHandler()
	{
		return m_Window->getEventHandler();
	}

}