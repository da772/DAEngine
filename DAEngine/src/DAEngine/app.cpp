#include "dapch.h"
#include "app.h"
#include "logger.h"
#include "core/arg_handler.h"
#include "core/memory/memory.h"

namespace da
{
	CApp::CApp(int argc, const char** argv) : m_running(true), m_modules() {
		memory::push_memory_layer(memory::EMemoryLayer::Core);
		initalizeInternal(argc, argv);
		memory::pop_memory_layer();
	}

	CApp::~CApp() {
		memory::push_memory_layer(memory::EMemoryLayer::Core);
		shutdownInternal();
		memory::pop_memory_layer();
	}

	void CApp::initalize()
	{
		memory::push_memory_layer(memory::EMemoryLayer::Core);
		for (IModule* m : m_modules) {
			m->initalize();
		}
		onInitalize();
		memory::pop_memory_layer();
	}
	void CApp::update()
	{
		memory::push_memory_layer(memory::EMemoryLayer::Core);
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
		memory::pop_memory_layer();
	}
	void CApp::shutdown()
	{
		memory::push_memory_layer(memory::EMemoryLayer::Core);
		for (IModule* m : m_modules) {
			m->shutdown();
		}
		onShutdown();
		for (IModule* m : m_modules) {
			m->lateShutdown();
			delete m;
		}
		memory::pop_memory_layer();
	}

	void CApp::addModule(IModule* module)
	{
		memory::push_memory_layer(memory::EMemoryLayer::Core);
		m_modules.push(module);
		memory::pop_memory_layer();
	}

	void CApp::forceEnd()
	{
		m_running = false;
	}

	void CApp::initalizeInternal(int argc, const char** argv)
	{
		CLogger::initialize();
		core::CArgHandler::initialize(argc, argv);
		CString args = "Initialized with argc: %d\n";
		for (size_t i = 0; i < argc; i++) {
			args += CString(argv[i]);
			if (i != argc - 1)
				args += "\n";
		}
		LOG_INFO(ELogChannel::Core, args, argc);
	}

	void CApp::shutdownInternal()
	{
		core::CArgHandler::shutdown();
		CLogger::shutdown();
	}

}