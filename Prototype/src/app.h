#pragma once

#include <daengine.h>
#include <iostream>
#include <functional>

#include <daengine/core/graphics/graphics_smesh.h>
#include <daengine/core/graphics/factory/factory_graphics_pipeline.h>
#include <daengine/core/graphics/factory/factory_graphics_texture2d.h>

class ProtoTypeApp : public da::CApp {

public:
	inline ProtoTypeApp(int argc, const char** argv) : CApp(argc, argv) {
		da::modules::CWindowModule* windowModule = new da::modules::CWindowModule({ "Hello World!", 720, 480, 0,0, 144, da::core::EWindowDeco::NONE });
		addModule(windowModule);
		
		windowModule->getEventHandler().registerCallback(EEventCategory::Window, BIND_EVENT_FN(ProtoTypeApp, windowEvent));

		m_graphicsModule = new da::modules::CGraphicsModule(*windowModule);
		addModule(m_graphicsModule);
		
		da::modules::CImGuiModule* imGuiModule = new da::modules::CImGuiModule(*m_graphicsModule);
		addModule(imGuiModule);

		//da::modules::CWindowModule* windowModule2 = new da::modules::CWindowModule({ "Hello World 2", 480, 240, 0,0, 144, da::platform::EWindowDeco::NONE });
		//addModule(windowModule2);
		//da::modules::CGraphicsModule* graphisModule2 = new da::modules::CGraphicsModule(*windowModule2);
		//addModule(graphisModule2);
		
		/*windowModule2->getEventHandler().registerCallback(EEventType::WindowClose, [windowModule2](const da::core::events::CEvent& e) {
			windowModule2->lateShutdown();
			});*/
	}

	inline ~ProtoTypeApp()
	{

	}

	inline void windowEvent(const da::core::events::CEvent& e) {

		if (e.getType() == EEventType::WindowClose) {
			da::CLogger::LogDebug(da::ELogChannel::Application, "WindowClosed!");
			forceEnd();
			return;
		}

		if (e.getType() == EEventType::WindowResize) {
			const CWindowResizeEvent* cl = static_cast<const CWindowResizeEvent*>(&e);
			da::CLogger::LogDebug(da::ELogChannel::Application, "WindowResize: {%d, %d}", cl->getWidth(), cl->getHeight());
			return;
		}

		if (e.getType() == EEventType::WindowMove) {
			const CWindowMoveEvent* cl = static_cast<const CWindowMoveEvent*>(&e);
			da::CLogger::LogDebug(da::ELogChannel::Application, "WindowMove: {%d, %d}", cl->getX(), cl->getY());
			return;
		}
	}
private:
	da::modules::CGraphicsModule* m_graphicsModule;

protected:
	inline virtual void onInitalize() override
	{
		//da::core::CModel model = da::core::CModel("assets/viking_room.obj");
		//da::core::CGraphicsPipeline* pipeline = da::core::CGraphicsPipelineFactory::Create(*m_graphicsModule->getGraphicsApi(), "shaders/vert.spv", "shaders/frag.spv", da::core::FVertexBase::getBindingDescription(), da::core::FVertexBase::getAttributeDescription());
		//pipeline->create();
		//da::core::CGraphicsTexture2D* texture = da::core::CTexture2DFactory::Create("assets/viking_room.png", *m_graphicsModule->getGraphicsApi());

	}

	inline virtual void onShutdown() override
	{
		da::CLogger::LogDebug(da::ELogChannel::Application, "App End");

	}

	inline virtual void onUpdate() override
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
