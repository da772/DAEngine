#include "dapch.h"
#include "app.h"
#include "logger.h"
#include "core/arg_handler.h"
#include "core/ecs/components.h"
#include "script/script_engine.h"

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
#ifndef DA_TEST
		script::CScriptEngine::initialize();
#endif
		for (IModule* m : m_modules) {
			m->initalize();
		}
		onInitalize();

		
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
			delete m;
		}
#ifndef DA_TEST
		script::CScriptEngine::shutdown();
#endif
	}

	void CApp::addModule(IModule* module)
	{
		m_modules.push_back(module);
	}

	void CApp::forceEnd()
	{
		m_running = false;
	}

	void CApp::initalizeInternal(int argc, const char** argv)
	{
		CLogger::initialize();
		core::CArgHandler::initialize(argc, argv);
		std::string args = "Initialized with argc: %d\n";
		for (size_t i = 0; i < argc; i++) {
			args += std::string(argv[i]);
			if (i != argc - 1)
				args += "\n";
		}
		LOG_INFO(ELogChannel::Core, args, argc);
		core::CComponents::registerComponents();
	}

	void CApp::shutdownInternal()
	{
		core::CArgHandler::shutdown();
		CLogger::shutdown();
	}

}