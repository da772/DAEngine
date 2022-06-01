#include "dapch.h"
#include "window_module.h"
#include "platform/window/window_factory.h"

namespace da::modules
{
	CWindowModule::CWindowModule(const platform::window::FWindowData& windowData) : m_Window(platform::window::CWindowFactory::CreateWindow(windowData))
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
		m_Window->shutdown();
	}

	const da::platform::CWindow& CWindowModule::getWindow() const
	{
		return *m_Window;
	}

}