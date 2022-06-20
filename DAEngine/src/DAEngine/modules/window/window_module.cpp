#include "dapch.h"
#include "window_module.h"
#include "core/window/window_factory.h"

namespace da::modules
{
	CWindowModule::CWindowModule(const core::FWindowData& windowData) : m_Window(core::CWindowFactory::CreateWindow(windowData))
	{

	}


	CWindowModule::~CWindowModule()
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

	const da::core::CWindow& CWindowModule::getWindow() const
	{
		return *m_Window;
	}

	void CWindowModule::lateShutdown()
	{
		m_Window->shutdown();
		delete m_Window;
	}

	da::core::events::CEventHandler& CWindowModule::getEventHandler()
	{
		return m_Window->getEventHandler();
	}

}