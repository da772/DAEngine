#pragma once

#include "daengine.h"
#include <iostream>

class ProtoTypeApp : public da::CApp {

public:

	ProtoTypeApp() {
		da::modules::CWindowModule* windowModule = new da::modules::CWindowModule({ "Hello World!", 720, 480, 0,0, 144, da::platform::window::EWindowDeco::NONE });
		addModule(windowModule);
	}

protected:
	virtual void onInitalize() override
	{
		std::cout << "Begin" << std::endl;
		da::CLogger::LogAssert(false, da::ELogChannel::External, "Hello World! %d", 123);
	}

	virtual void onShutdown() override
	{
		std::cout << "End" << std::endl;
	}

	virtual void onUpdate() override
	{
		da::CLogger::LogTrace(da::ELogChannel::External, "Hello World! %d", 123);
		da::CLogger::LogDebug(da::ELogChannel::External, "Hello World! %d", 123);
		da::CLogger::LogWarning(da::ELogChannel::External, "Hello World! %d", 123);
		da::CLogger::LogError(da::ELogChannel::External, "Hello World! %d", 123);
		
		//std::cout << "Run" << std::endl;
	}
};
