#pragma once

#include <daengine.h>
#include <iostream>
#include <functional>

#include <daengine/core/graphics/graphics_smesh.h>
#include <daengine/core/graphics/factory/factory_graphics_pipeline.h>
#include <daengine/core/graphics/factory/factory_graphics_texture2d.h>

#include <imgui.h>

//#define WINDOW_2

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
#ifdef WINDOW_2
		da::modules::CWindowModule* windowModule2 = new da::modules::CWindowModule({ "Hello World 2", 480, 240, 0,0, 144, da::core::EWindowDeco::NONE });
		addModule(windowModule2);
		m_graphicsModule2 = new da::modules::CGraphicsModule(*windowModule2);
		addModule(m_graphicsModule2);
		
		windowModule2->getEventHandler().registerCallback(EEventType::WindowClose, [windowModule2](const da::core::events::CEvent& e) {
			windowModule2->lateShutdown();
			});
#endif
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
	da::modules::CGraphicsModule* m_graphicsModule2;
	da::core::CMaterial* m_boltMat = 0;

protected:
	inline virtual void onInitalize() override
	{
		
		auto pipeline = da::core::CGraphicsPipelineFactory::CreatePBR(*m_graphicsModule->getGraphicsApi());

		m_graphicsModule->getGraphicsApi()->submitPipeline(pipeline);
		
		
		da::core::CStaticMesh* model = new  da::core::CStaticMesh("assets/viking_room.obj");
		da::core::CStaticMesh* model3 = new da::core::CStaticMesh("assets/coffee.fbx");
		da::core::CStaticMesh* model4 = new da::core::CStaticMesh("assets/bolt.fbx");
		da::core::CMaterial* mat1 = da::core::CMaterialFactory::CreatePBR(*pipeline, "assets/viking_room.png", "assets/viking_room.png");
		da::core::CMaterial* mat2 = da::core::CMaterialFactory::CreatePBR(*pipeline, "assets/penguin.png", "assets/viking_room.png");
		da::core::CMaterial* mat3 = da::core::CMaterialFactory::CreatePBR(*pipeline, "assets/coffeeA.png", "assets/coffeeN.png", "assets/coffeeR.png", "assets/coffeeM.png");
		m_boltMat = da::core::CMaterialFactory::CreatePBR(*pipeline, "assets/boltA.jpg", "assets/boltN.png", "assets/boltR.jpg", "assets/boltM.jpg", "assets/boltAO.jpg");
		da::core::CMaterial* mat4 = da::core::CMaterialFactory::CreatePBR(*pipeline, "assets/boltA.jpg");
		

		mat1->Position = da::Vector3f(0.f, 0.f, 0.f);
		mat2->Position = da:: Vector3f(1.f, 0.f, 0.f);
		mat3->Position = da::Vector3f(0.f, 0.f, 0.f);
		m_boltMat->Position = da::Vector3f(0.55f, 0.3f, 0.3f);
		//pipeline->addRenderable(model, mat1);
		//pipeline->addRenderable(model2, mat2);
		//pipeline->addRenderable(model3, mat3);
		pipeline->addRenderable(model4, m_boltMat);
		
		/*
		auto cubeMapPipeline = da::core::CGraphicsPipelineFactory::CreateCubeMap(*m_graphicsModule->getGraphicsApi());
		m_graphicsModule->getGraphicsApi()->submitPipeline(cubeMapPipeline);

		da::core::CStaticMesh* skybox = new da::core::CStaticMesh("assets/skybox.obj");
		da::core::CMaterial* cubeMapMat = da::core::CMaterialFactory::CreateCubeMap(*cubeMapPipeline, "assets/environment.hdr");
		cubeMapPipeline->addRenderable(skybox, cubeMapMat);
		*/
		
#ifdef WINDOW_2

		auto pipeline2 = da::core::CGraphicsPipelineFactory::Create(*m_graphicsModule2->getGraphicsApi(), "shaders/vert.spv", "shaders/frag.spv",
			da::core::FVertexBase::getBindingDescription(), da::core::FVertexBase::getAttributeDescription());

		m_graphicsModule2->getGraphicsApi()->submitPipeline(pipeline2);

		da::core::CMaterial* mat4 = da::core::CMaterialFactory::Create(*pipeline2, "assets/penguin.png", "assets/viking_room.png");
		pipeline2->addRenderable(model2, mat4);
#endif
	}

	inline virtual void onShutdown() override
	{
		da::CLogger::LogDebug(da::ELogChannel::Application, "App End");

	}

	inline virtual void onUpdate() override
	{
		if (!m_boltMat)
		{
			return;
		}
		if (ImGui::Begin("Object Viewer"))
		{
			ImGui::Text("Bolt Cutter");
			ImGui::Text("Position");
			ImGui::SameLine();
			ImGui::DragFloat3("##Position", (float*)m_boltMat->Position.data(), .1f);

		}

		ImGui::End();
		
	}
};
