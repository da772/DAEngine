#pragma once

#include "core/containers.h"
#include "modules/module.h"

namespace da
{
	class CApp
	{
	public:
		CApp(int argc, const char** argv);
		virtual ~CApp();
		void initialize();
		void update();
		void shutdown();
		void reset();

	protected:
		virtual void onInitialize() = 0;
		virtual void onUpdate() = 0;
		virtual void onShutdown() = 0;

		void addModule(IModule* module);
		void forceEnd();
	private:
		void initalizeInternal(int argc, const char** argv);
		void shutdownInternal();

	private:
		bool m_running;
		bool m_initialized = false;
		std::vector<IModule*> m_modules;
		bool m_reset = false;
	};
	
	extern CApp* createApp(int argc, const char** argv);
}
