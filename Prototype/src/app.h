#pragma once

#include <daengine.h>
#include <iostream>
#include <functional>

#include <daengine/core/graphics/graphics_smesh.h>
#include <daengine/core/graphics/factory/factory_graphics_pipeline.h>
#include <daengine/core/graphics/factory/factory_graphics_texture2d.h>
#include "DAEngine/core/graphics/graphics_smesh_cube.h"

#include <imgui.h>

#include <bgfx/bgfx.h>

//#define WINDOW_2

#define BX_TRACE(...) da::CLogger::LogInfo(da::ELogChannel::Graphics, ...);

class ProtoTypeApp : public da::CApp {

public:
	inline ProtoTypeApp(int argc, const char** argv) : CApp(argc, argv) {
		m_window = new da::modules::CWindowModule({ "Hello World!", 720, 480, 0,0, 144, da::core::EWindowDeco::NONE });
		addModule(m_window);

		m_window->getEventHandler().registerCallback(EEventCategory::Window, BIND_EVENT_FN(ProtoTypeApp, windowEvent));

		//m_graphicsModule = new da::modules::CGraphicsModule(*windowModule);
		//addModule(m_graphicsModule);
		
		//da::modules::CImGuiModule* imGuiModule = new da::modules::CImGuiModule(*m_graphicsModule);
		//addModule(imGuiModule);
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
	da::modules::CWindowModule* m_window = 0;

protected:
	inline virtual void onInitalize() override
	{
		// Initialize bgfx using the native window handle and window resolution.
		bgfx::Init init;
		init.type = bgfx::RendererType::Enum::Direct3D11;
		bgfx::PlatformData pd;
		pd.nwh = m_window->getWindow().getPlatformWindow();
		pd.ndt = NULL;
		init.platformData = pd;
		init.resolution.width = 720;
		init.resolution.height = 480;
		init.resolution.reset = 144;
		
		if (!bgfx::init(init))
		{
			da::CLogger::LogError(da::ELogChannel::Application, "Failed to create bgfx");
		}

		// Enable debug text.
		bgfx::setDebug(BGFX_DEBUG_TEXT);

		bgfx::setViewClear(0
			, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
			, 0x303030ff
			, 1.0f
			, 0
		);
		bgfx::setViewRect(0, 0, 0, 720, 480);

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
		bgfx::setViewRect(0, 0, 0, uint16_t(720), uint16_t(480));
		bgfx::touch(0);
		bgfx::dbgTextClear();
		bgfx::dbgTextPrintf(0, 1, 0x0f, "Color can be changed with ANSI \x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m code too.");
		bgfx::dbgTextPrintf(80, 1, 0x0f, "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; 5m    \x1b[; 6m    \x1b[; 7m    \x1b[0m");
		bgfx::dbgTextPrintf(80, 2, 0x0f, "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    \x1b[;13m    \x1b[;14m    \x1b[;15m    \x1b[0m");
		bgfx::frame();
		return;
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
