
#include "window_module.h"
#include "platform/window/factory/window_factory.h"
#include "core/input/input.h"


namespace da::modules
{
	CWindowModule::CWindowModule(const core::FWindowData& windowData)
	{
		m_Window = factory::CWindowFactory::CreateWindow(windowData);
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
		da::core::CInput::registerWindow(m_Window);
	}

	void CWindowModule::update()
	{
		PROFILE()
		m_Window->update();
	}

	void CWindowModule::shutdown()
	{
		da::core::CInput::unregisterWindow(m_Window);
	}

	da::core::CWindow* CWindowModule::getWindow() const
	{
		return m_Window;
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