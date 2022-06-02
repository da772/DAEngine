#include "dapch.h"
#include "app.h"
#include "logger.h"
#include "core/arg_handler.h"

namespace da
{
	CApp::CApp(int argc, const char** argv) : m_running(true), m_modules() {
		initalizeInternal(argc, argv);
	}

	CApp::~CApp() {
		shutdownInternal();
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
	}
	void CApp::shutdown()
	{
		for (IModule* m : m_modules) {
			m->shutdown();
		}
		onShutdown();
		for (IModule* m : m_modules) {
			m->lateShutdown();
		}
	}

	void CApp::addModule(IModule* module)
	{
		m_modules.push(module);
	}

	void CApp::forceEnd()
	{
		m_running = false;
	}

	void CApp::initalizeInternal(int argc, const char** argv)
	{
		CLogger::initialize();
		core::CArgHandler::initialize(argc, argv);
	}

	void CApp::shutdownInternal()
	{
		core::CArgHandler::shutdown();
		CLogger::shutdown();
	}

}