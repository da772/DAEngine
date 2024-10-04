
#include "window_module.h"
#include "platform/window/factory/window_factory.h"
#include "core/input/input.h"


namespace da
{
	CWindowModule::CWindowModule(const FWindowData& windowData)
	{
		m_Window = CWindowFactory::CreateWindow(windowData);
	}


	CWindowModule::~CWindowModule()
	{

	}

	void CWindowModule::lateUpdate()
	{

	}

	void CWindowModule::initialize()
	{
		m_Window->initialize();
		da::CInput::registerWindow(m_Window);
	}

	void CWindowModule::update()
	{
		PROFILE()
		m_Window->update();
	}

	void CWindowModule::shutdown()
	{
		da::CInput::unregisterWindow(m_Window);
	}

	da::CWindow* CWindowModule::getWindow() const
	{
		return m_Window;
	}

	void CWindowModule::lateShutdown()
	{
		m_Window->shutdown();
		delete m_Window;
	}

	da::CEventHandler& CWindowModule::getEventHandler()
	{
		return m_Window->getEventHandler();
	}

}