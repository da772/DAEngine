#pragma once

#include "daengine.h"
#include <iostream>
#include <functional>

class ProtoTypeApp : public da::CApp {

public:
	
	ProtoTypeApp(int argc, const char** argv) : CApp(argc, argv) {
		da::modules::CWindowModule* windowModule = new da::modules::CWindowModule({ "Hello World!", 720, 480, 0,0, 144, da::platform::window::EWindowDeco::NONE });
		addModule(windowModule);
		da::modules::CGraphicsModule* graphicsModule = new da::modules::CGraphicsModule(windowModule);
		addModule(graphicsModule);

		da::modules::CWindowModule* windowModule2 = new da::modules::CWindowModule({ "Hello World 2", 480, 240, 0,0, 144, da::platform::window::EWindowDeco::NONE });
		addModule(windowModule2);
		
		windowModule->getEventHandler().registerCallback(EEventCategory::Window,  BIND_EVENT_FN(ProtoTypeApp, windowEvent));
		windowModule2->getEventHandler().registerCallback(EEventType::WindowClose, [windowModule2](const da::core::events::CEvent& e) {
			windowModule2->lateShutdown();
		});
	}

	void windowEvent(const da::core::events::CEvent& e) {

		if (e.getType() == EEventType::WindowClose) {
			da::CLogger::LogDebug(da::ELogChannel::External, "WindowClosed!");
			forceEnd();
			return;
		}

		if (e.getType() == EEventType::WindowResize) {
			const CWindowResizeEvent* cl = static_cast<const CWindowResizeEvent*>(&e);
			da::CLogger::LogDebug(da::ELogChannel::External, "WindowResize: {%d, %d}", cl->getWidth(), cl->getHeight());
			return;
		}

		if (e.getType() == EEventType::WindowMove) {
			const CWindowMoveEvent* cl = static_cast<const CWindowMoveEvent*>(&e);
			da::CLogger::LogDebug(da::ELogChannel::External, "WindowMove: {%d, %d}", cl->getX(), cl->getY());
			return;
		}
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
