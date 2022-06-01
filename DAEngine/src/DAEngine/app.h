#pragma once

#include "core/containers.h"
#include "modules/module.h"

namespace da
{
	class CApp
	{
	public:
		CApp();
		~CApp();
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
		void initalizeInternal();
		void shutdownInternal();

	private:
		bool m_running;
		TList<IModule*> m_modules;
	};
	
	extern CApp* createApp();
}
