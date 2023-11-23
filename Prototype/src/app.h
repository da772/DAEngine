#pragma once

#include <daengine.h>
#include <iostream>
#include <functional>

#include <daengine/core/graphics/graphics_smesh.h>
#include <daengine/core/graphics/factory/factory_graphics_pipeline.h>
#include <daengine/core/graphics/factory/factory_graphics_texture2d.h>
#include "DAEngine/core/graphics/graphics_smesh_cube.h"

#include <daengine/core/ecs/scene.h>
#include <daengine/core/ecs/entity.h>
#include <daengine/core/ecs/test_component.h>
#include <daengine/core/ecs/script_component.h>

#include <imgui.h>

#include <daengine/script/script_engine.h>
#include <DAEngine/core/ecs/scene_manager.h>
#include <DAEngine/core/graphics/camera.h>
#include <DAEngine/core/input/input.h>

//#define WINDOW_2

class ProtoTypeApp : public da::CApp {

public:
	inline ProtoTypeApp(int argc, const char** argv) : CApp(argc, argv) {
		m_window = new da::modules::CWindowModule({ "Hello World!", 720, 480, 0,0, 144, da::core::EWindowDeco::NONE });
		addModule(m_window);

		m_window->getEventHandler().registerCallback(EEventCategory::Window, BIND_EVENT_FN(ProtoTypeApp, windowEvent));

		m_graphicsModule = new da::modules::CGraphicsModule(m_window);
		addModule(m_graphicsModule);

		da::modules::CImGuiModule* imGuiModule = new da::modules::CImGuiModule(m_graphicsModule);
		addModule(imGuiModule);

		da::core::CCamera::setCamera(new da::core::CCamera());

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
			//da::CLogger::LogDebug(da::ELogChannel::Application, "WindowResize: {%d, %d}", cl->getWidth(), cl->getHeight());
			return;
		}

		if (e.getType() == EEventType::WindowMove) {
			const CWindowMoveEvent* cl = static_cast<const CWindowMoveEvent*>(&e);
			//da::CLogger::LogDebug(da::ELogChannel::Application, "WindowMove: {%d, %d}", cl->getX(), cl->getY());
			return;
		}
	}

private:
	da::modules::CGraphicsModule* m_graphicsModule;
	da::modules::CGraphicsModule* m_graphicsModule2;
	da::core::CMaterial* m_boltMat = 0;
	da::core::CMaterial* m_cubeMat = 0;
	da::core::CMaterial* m_cubeMat2 = 0;
	da::modules::CWindowModule* m_window = 0;
	da::core::CEntity* e1,* e2;
    da::core::FComponentRef<da::core::CScriptComponent> scriptComponent;

protected:
	inline virtual void onInitalize() override
	{
		da::core::CSceneManager::setScene(new da::core::CScene(da::core::CGuid::Generate()));
		e1 = da::core::CSceneManager::getScene()->createEntity();
		da::core::FComponentRef<da::core::CTestComponent> tst1 = e1->addComponent<da::core::CTestComponent>("helloworld1", "helloworld2");
		tst1->data1 = "123456";
		da::core::FComponentRef<da::core::CTestComponent> tst11 = e1->getComponent<da::core::CTestComponent>();
		tst11->data1 = "883818";
		tst1->initialize();
		LOG_DEBUG(da::ELogChannel::Application, "%s, %s", tst1->data1.c_str(), tst1->data2.c_str());
		e2 = da::core::CSceneManager::getScene()->createEntity();
		da::core::FComponentRef<da::core::CTestComponent> tst2 = e2->addComponent<da::core::CTestComponent>("helloworld3", "helloworld4");
		da::core::FComponentRef<da::core::CTestComponent> tst22 = e1->getComponent<da::core::CTestComponent>();
		tst22->data1 = "883818";
		LOG_DEBUG(da::ELogChannel::Application, "%s, %s, %s", tst2->data1.c_str(), tst2->data2.c_str(), tst22->data1.c_str());

        scriptComponent = e2->addComponent<da::core::CScriptComponent>("scripts/helloworld.lua");

		//m_graphicsModule->getGraphicsApi()->setClearColor(0, da::core::EGraphicsClear::Color | da::core::EGraphicsClear::Depth, { 255,0,0,255 });
		return;
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
				da::core::CStaticMesh* modelCube = new da::core::CStaticMeshCube();
				m_boltMat = da::core::CMaterialFactory::CreatePBR(*pipeline, "assets/boltA.jpg", "assets/boltN.png", "assets/boltR.jpg", "assets/boltM.jpg", "assets/boltAO.jpg");
				m_cubeMat2 = da::core::CMaterialFactory::CreatePBR(*pipeline);

				m_boltMat->Position = da::Vector3f(0.5f, 0.0f, -.5f);
				m_boltMat->Scale = da::Vector3f(0.25f, 0.25f, .25f);
				m_boltMat->RotationSpeed = 0.f;//20.f;
				m_boltMat->Rotation.z = 90.f;
				m_cubeMat2->Position = da::Vector3f(0.0f, 0.f, 0.f);
				m_cubeMat2->Scale = da::Vector3f(1.f , 1.f, 1.f);
				//pipeline->addRenderable(model, mat1);
				//pipeline->addRenderable(model2, mat2);
				//pipeline->addRenderable(model3, at3);
				pipeline->addRenderable(model4, m_boltMat);
				//pipeline->addRenderable(modelCube, m_cubeMat2);
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

		da::core::CCamera& cam = *da::core::CCamera::getCamera();

		if (da::core::CInput::inputPressed(87)) // W
		{
			cam.move(cam.forward() * 5.0f * .1f);
		}

		if (da::core::CInput::inputPressed(83)) // S
		{
			cam.move(-cam.forward() * 5.0f * .1f);
		}

		if (da::core::CInput::inputPressed(65)) // A
		{
			cam.move(-cam.right() * 5.0f * .1f);
		}
		
		if (da::core::CInput::inputPressed(68)) // D
		{
			cam.move(cam.right() * 5.0f * .1f);
		}

		if (da::core::CInput::inputPressed(32)) // SPACE
		{
			cam.move(cam.up() * 5.0f * .1f);
		}

		if (da::core::CInput::inputPressed(341)) // LCTRL
		{
			cam.move(-cam.up() * 5.0f * .1f);
		}

        if (ImGui::Begin("Scripts")) {
            if (ImGui::Button("Reload")) {
                auto& components = da::core::CSceneManager::getScene()->getComponents<da::core::CScriptComponent>();
                da::script::CScriptEngine::clear_all();
                for (size_t i = 0; i < components.getCount(); i++) {
                    da::core::CScriptComponent* c = (da::core::CScriptComponent*)components.getComponentAtIndex(i);
                    c->reload();
                }
            }
        }
        
        ImGui::End();
        
		return;
		if (!m_boltMat || !m_cubeMat)
		{
			return;
		}
		if (ImGui::Begin("Object Viewer"))
		{
			ImGui::Text("Bolt Cutter");
			ImGui::Text("Position123");
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
