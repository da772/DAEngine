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
		void initalize();
		void update();
		void shutdown();

	protected:
		virtual void onInitalize() = 0;
		virtual void onUpdate() = 0;
		virtual void onShutdown() = 0;

		void addModule(IModule* module);
		void forceEnd();
	private:
		void initalizeInternal(int argc, const char** argv);
		void shutdownInternal();

	private:
		bool m_running;
		std::vector<IModule*> m_modules;
	};
	
	extern CApp* createApp(int argc, const char** argv);
}
