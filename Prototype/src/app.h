#pragma once

#include <daengine.h>
#include <iostream>
#include <functional>

#include <daengine/core/graphics/graphics_smesh.h>
#include <daengine/core/graphics/factory/factory_graphics_pipeline.h>
#include <daengine/core/graphics/factory/factory_graphics_texture2d.h>
#include "DAEngine/core/graphics/graphics_smesh_cube.h"

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
		da::modules::CWindowModule* windowModule2 = new da::modules::CWindowModule({ "Hello World 2", 720, 480, 0,0, 144, da::core::EWindowDeco::NONE });
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
	da::core::CMaterial* m_cubeMat = 0;
	da::core::CMaterial* m_cubeMat2 = 0;

protected:
	inline virtual void onInitalize() override
	{
		{
			
			{
				auto cubeMapPipeline = da::core::CGraphicsPipelineFactory::CreateCubeMap(*m_graphicsModule->getGraphicsApi());
				m_graphicsModule->getGraphicsApi()->submitPipeline(cubeMapPipeline);
				da::core::CStaticMesh* skybox = new da::core::CStaticMeshCube();
				m_cubeMat = da::core::CMaterialFactory::CreateCubeMap(*cubeMapPipeline, "assets/cubemap.ktx");
				cubeMapPipeline->addRenderable(skybox, m_cubeMat);
			}

			{
				auto pipeline = da::core::CGraphicsPipelineFactory::CreatePBR(*m_graphicsModule->getGraphicsApi());
				m_graphicsModule->getGraphicsApi()->submitPipeline(pipeline);
				da::core::CStaticMesh* model4 = new da::core::CStaticMesh("assets/bolt.fbx");
				da::core::CStaticMesh* modelCube = new da::core::CStaticMesh("assets/cube.obj");
				m_boltMat = da::core::CMaterialFactory::CreatePBR(*pipeline, "assets/boltA.jpg", "assets/boltN.png", "assets/boltR.jpg", "assets/boltM.jpg", "assets/boltAO.jpg");
				m_cubeMat2 = da::core::CMaterialFactory::CreatePBR(*pipeline, "assets/ceramic/ceramicA.png", "assets/ceramic/ceramicN.png", 
					"assets/white.png", "assets/ceramic/ceramicM.png", "assets/ceramic/ceramicAO.png");

				m_boltMat->Position = da::Vector3f(0.5f, 0.0f, -.5f);
				m_boltMat->Scale = da::Vector3f(0.25f, 0.25f, .25f);
				m_boltMat->RotationSpeed = 20.f;
				m_boltMat->Rotation.z = 90.f;
				m_cubeMat2->Position = da::Vector3f(0.0f, -1.5f, 0.f);
				m_cubeMat2->Scale = da::Vector3f(1.f, 1.f, 1.f);
				//pipeline->addRenderable(model, mat1);
				//pipeline->addRenderable(model2, mat2);
				//pipeline->addRenderable(model3, at3);
				pipeline->addRenderable(model4, m_boltMat);
				pipeline->addRenderable(modelCube, m_cubeMat2);
			}
		}
		
#ifdef WINDOW_2
		{
			auto cubeMapPipeline = da::core::CGraphicsPipelineFactory::CreateCubeMap(*m_graphicsModule2->getGraphicsApi());
			m_graphicsModule2->getGraphicsApi()->submitPipeline(cubeMapPipeline);

			da::core::CStaticMesh* skybox = new da::core::CStaticMeshCube();
			auto cubeMat = da::core::CMaterialFactory::CreateCubeMap(*cubeMapPipeline, "assets/cubemap_yokohama_rgba.ktx");
			cubeMapPipeline->addRenderable(skybox, cubeMat);

			auto pipeline2 = da::core::CGraphicsPipelineFactory::CreatePBR(*m_graphicsModule2->getGraphicsApi());
			m_graphicsModule2->getGraphicsApi()->submitPipeline(pipeline2);

			da::core::CStaticMesh* model3 = new da::core::CStaticMesh("assets/coffee.fbx");

			da::core::CMaterial* mat1 = da::core::CMaterialFactory::CreatePBR(*pipeline2, "assets/coffeeA.png", "assets/coffeeN.png", "assets/coffeeR.png", "assets/coffeeM.png");
			mat1->Position = { 0.f, 0.f, .35f };
			mat1->CamRot = { 0.f, 180.f, 0.f };
			pipeline2->addRenderable(model3, mat1);
		}
#endif
	}

	inline virtual void onShutdown() override
	{
		da::CLogger::LogDebug(da::ELogChannel::Application, "App End");

	}

	inline virtual void onUpdate() override
	{
		if (!m_boltMat || !m_cubeMat)
		{
			return;
		}
		if (ImGui::Begin("Object Viewer"))
		{
			ImGui::Text("Bolt Cutter");
			ImGui::Text("Position");
			ImGui::SameLine();
			if (ImGui::DragFloat3("##Position", (float*)m_boltMat->Position.data(), .1f))
			{
			}

			ImGui::Text("Camera");
			ImGui::Text("Position");
			ImGui::SameLine();
			if (ImGui::DragFloat3("##CamPosition", (float*)m_boltMat->CamPosition.data(), .1f))
			{
				m_cubeMat->CamPosition = m_boltMat->CamPosition;
				m_cubeMat2->CamPosition = m_boltMat->CamPosition;
			}

			ImGui::Text("Rotation");
			ImGui::SameLine();
			if (ImGui::DragFloat3("##CamRotation", (float*)m_boltMat->CamRot.data(), .1f))
			{
				m_cubeMat->CamRot = m_boltMat->CamRot;
				m_cubeMat2->CamRot = m_boltMat->CamRot;
			}

		}

		ImGui::End();
		
	}
};
