#pragma once

#include "daengine.h"
#include <iostream>

class ProtoTypeApp : public da::CApp {

public:

	ProtoTypeApp(int argc, const char** argv) : CApp(argc, argv) {
		da::modules::CWindowModule* windowModule = new da::modules::CWindowModule({ "Hello World!", 720, 480, 0,0, 144, da::platform::window::EWindowDeco::NONE });
		addModule(windowModule);
		da::modules::CGraphicsModule* graphicsModule = new da::modules::CGraphicsModule(windowModule);
		addModule(graphicsModule);
	}

protected:
	virtual void onInitalize() override
	{
		da::CLogger::LogDebug(da::ELogChannel::External, "App Init");
	}

	virtual void onShutdown() override
	{
		da::CLogger::LogDebug(da::ELogChannel::External, "App End");
	}

	virtual void onUpdate() override
	{
		/*
		da::CLogger::LogTrace(da::ELogChannel::External, "Hello World! %d", 123);
		da::CLogger::LogDebug(da::ELogChannel::External, "Hello World! %d", 123);
		da::CLogger::LogWarning(da::ELogChannel::External, "Hello World! %d", 123);
		da::CLogger::LogError(da::ELogChannel::External, "Hello World! %d", 123);
		*/
		
		//std::cout << "Run" << std::endl;
	}
};
