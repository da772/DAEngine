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

	}

	virtual void onShutdown() override
	{
		std::cout << "End" << std::endl;
	}

	virtual void onUpdate() override
	{
		std::cout << "Run" << std::endl;
	}
};
