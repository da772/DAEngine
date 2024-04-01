#include "dapch.h"
#include "app.h"
#include "logger.h"
#include "core/arg_handler.h"
#include "core/ecs/components.h"
#include "core/ecs/scene.h"
#include "script/script_engine.h"
#include "core/ecs/scene_manager.h"
#include "core/threading/worker_pool.h"
#include "core/time.h"

#include "physics/physics.h"

#ifdef DA_REVIEW
#include "debug/debug.h"
#include "debug/debug_menu_bar.h"
#endif
#include <core/graphics/animation_manager.h>

namespace da
{
	CApp::CApp(int argc, const char** argv) : m_running(true), m_modules() {
		initalizeInternal(argc, argv);
	}

	CApp::~CApp() {
		shutdownInternal();
	}

	void CApp::initialize()
	{
		m_initialized = true;
#ifndef DA_TEST
		script::CScriptEngine::initialize();
#ifdef DA_REVIEW
		da::debug::CDebug::initialize();
		da::debug::CDebugMenuBar::register_debug(HASHSTR("App"), HASHSTR("Reset"), &m_reset, [&] { reset();});
#endif
#endif

		core::CSceneManager::initialize();
		da::physics::CPhysics::initialize();
		for (IModule* m : m_modules) {
			m->initialize();
		}
		onInitialize();
		if (core::CScene* scene = core::CSceneManager::getScene()) {
			scene->initialize();
		}
		
	}
	void CApp::update()
	{
		while (m_running)
		{
			double timeStep = da::core::CTime::newFrame();
			da::graphics::CAnimationManager::updateBegin(timeStep);
			for (IModule* m : m_modules) {
				m->update();
			}
#ifdef DA_REVIEW
			da::debug::CDebug::update(timeStep);
#endif
			onUpdate(timeStep);
			if (core::CScene* scene = core::CSceneManager::getScene()) {
				scene->update(timeStep);
			}

			da::physics::CPhysics::update(timeStep);

			da::graphics::CAnimationManager::updateEnd();
			for (IModule* m : m_modules) {
				m->lateUpdate();
			}
			
			if (m_reset) {
				shutdown();
				initialize();
				m_reset = false;
			}
		}
 	}
	void CApp::shutdown()
	{
		onShutdown();

		if (core::CScene* scene = core::CSceneManager::getScene()) {
			scene->shutdown();
			delete scene;
			core::CSceneManager::setScene(nullptr);
		}

		for (IModule* m : m_modules) {
			m->shutdown();
		}

		for (IModule* m : m_modules) {
			m->lateShutdown();
			delete m;
		}
		m_modules = {};
		da::physics::CPhysics::shutdown();
		core::CSceneManager::shutdown();
#ifndef DA_TEST
		script::CScriptEngine::shutdown();
#ifdef DA_REVIEW
		da::debug::CDebugMenuBar::unregister_debug(HASHSTR("App"), HASHSTR("Reset"));
		da::debug::CDebug::shutdown();
#endif
#endif
	}

	void CApp::reset()
	{
		m_reset = true;
	}

	void CApp::addModule(IModule* module)
	{
		m_modules.push_back(module);
		if (m_initialized) module->initialize();
	}

	void CApp::forceEnd()
	{
		m_running = false;
	}

	void CApp::initalizeInternal(int argc, const char** argv)
	{
		CLogger::initialize();
		core::CWorkerPool::initialize();
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
		core::CWorkerPool::shutdown();
	}

}