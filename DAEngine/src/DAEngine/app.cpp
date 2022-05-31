#include "dapch.h"
#include "app.h"

namespace da
{
	CApp::CApp() : m_running(true), m_modules() {

	}

	CApp::~CApp() {

	}

	void CApp::initalize()
	{
		onInitalize();
		for (IModule* m : m_modules) {
			m->initalize();
		}
	}
	void CApp::update()
	{
		while (m_running)
		{
			for (IModule* m : m_modules) {
				m->update();
			}
			onUpdate();
			for (IModule* m : m_modules) {
				m->lateUpdate();
			}
		}

		shutdown();
	}
	void CApp::shutdown()
	{
		for (IModule* m : m_modules) {
			m->shutdown();
		}
		onShutdown();
	}

	void CApp::addModule(IModule* module)
	{
		m_modules.push(module);
	}

	void CApp::forceEnd()
	{
		m_running = false;
	}

}