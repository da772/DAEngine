#include "dapch.h"
#include "app.h"
#include "logger.h"
#include "core/arg_handler.h"
#include "core/ecs/components.h"
#include "core/ecs/scene.h"
#include "script/script_engine.h"
#include "core/ecs/scene_manager.h"

#ifdef DA_DEBUG
#include "debug/debug.h"
#endif

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
#ifdef DA_DEBUG
		da::debug::CDebug::initialize();
#endif
#endif



		for (IModule* m : m_modules) {
			m->initalize();
		}
		onInitalize();
		if (core::CScene* scene = core::CSceneManager::getScene()) {
			scene->initialize();
		}
		
	}
	void CApp::update()
	{
		while (m_running)
		{
			for (IModule* m : m_modules) {
				m->update();
			}
#ifdef DA_DEBUG
			da::debug::CDebug::update(0.1f);
#endif
			onUpdate();
			if (core::CScene* scene = core::CSceneManager::getScene()) {
				scene->update(0.1f);
			}
			for (IModule* m : m_modules) {
				m->lateUpdate();
			}
		}
	}
	void CApp::shutdown()
	{
		if (core::CScene* scene = core::CSceneManager::getScene()) {
			scene->shutdown();
		}
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
#ifdef DA_DEBUG
		da::debug::CDebug::shutdown();
#endif
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
		core::CSceneManager::initialize();
		core::CComponents::registerComponents();
	}

	void CApp::shutdownInternal()
	{
		core::CArgHandler::shutdown();
		CLogger::shutdown();
		core::CSceneManager::shutdown();
	}

}