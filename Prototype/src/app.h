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
#include <daengine/core/ecs/smesh_component.h>

#include <imgui.h>

#include <daengine/script/script_engine.h>
#include <DAEngine/core/ecs/scene_manager.h>
#include <DAEngine/core/graphics/camera.h>
#include <DAEngine/core/input/input.h>
#include <DAEngine/platform/graphics/bgfx/bgfx_graphics_pbr_material.h>
#include <daengine/core/graphics/graphics_skmesh.h>

#include <DAEngine\platform\graphics\bgfx\bgfx_skeletal_mesh.h>
#include <DAEngine\core\graphics\skeletal_animation.h>
#include <DAEngine\core\graphics\skeletal_animator.h>

#include <glm/gtx/matrix_decompose.hpp>


#ifdef DA_DEBUG
#include <DAEngine/debug/debug_menu_bar.h>
#endif
#include <DAEngine/core/ecs/skeletal_mesh_component.h>
#include <DAEngine/core/time.h>
#include <glm/gtx/quaternion.hpp>

//#define WINDOW_2

class ProtoTypeApp : public da::CApp {

public:
	inline ProtoTypeApp(int argc, const char** argv) : CApp(argc, argv) {
		
	}

	inline ~ProtoTypeApp()
	{

	}

	inline void createModules()
	{
		m_window = new da::modules::CWindowModule({ "Hello World!", 1280, 720, 0,0, 144, da::core::EWindowDeco::NONE });
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
	da::graphics::CMaterial* m_boltMat = 0;
	da::graphics::CMaterial* m_cubeMat = 0;
	da::graphics::CMaterial* m_cubeMat2 = 0;
	da::modules::CWindowModule* m_window = 0;
	da::core::CEntity* e1,* e2, *e3, *e4, *e5;
	bool m_showScriptDebug = false;
	bool m_showScriptDebugHard = false;
 

protected:
	inline virtual void onInitialize() override
	{
		createModules();
#ifdef DA_DEBUG
		da::debug::CDebugMenuBar::register_debug(HASHSTR("Script"), HASHSTR("Reload Scripts"), &m_showScriptDebug, [&] {renderScriptDebug(true, &m_showScriptDebug); });
		da::debug::CDebugMenuBar::register_debug(HASHSTR("Script"), HASHSTR("Reload Scripts (Hard)"), &m_showScriptDebugHard, [&] {renderScriptDebug(false, &m_showScriptDebugHard); });
#endif

		da::core::CSceneManager::setScene(new da::core::CScene(da::core::CGuid::Generate()));
		
		e1 = da::core::CSceneManager::getScene()->createEntity();
		e1->setTag(HASHSTR("sniper"));
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

        e2->addComponent<da::core::CScriptComponent>("scripts/build/helloworld.lua", "MyComponent");
		e1->addComponent<da::core::CScriptComponent>("scripts/build/camera_component.lua", "CameraComponent");
		e1->getTransform().setPosition({ 0,0,25.5f });
		da::core::FComponentRef<da::core::CSmeshComponent> c = e1->addComponent<da::core::CSmeshComponent>("assets/sniper/Sniper.fbx");
		c->getStaticMesh()->getMaterial(0).baseColorTexture = da::graphics::CTexture2DFactory::Create("assets/sniper/Textures/Variation04/Sniper_04_Albedo.png");
		c->getStaticMesh()->getMaterial(0).normalTexture = da::graphics::CTexture2DFactory::Create("assets/sniper/Textures/Shared/Sniper_Normal.png");
		c->getStaticMesh()->getMaterial(0).metallicRoughnessTexture = da::graphics::CTexture2DFactory::Create("assets/sniper/Textures/Variation04/Sniper_05_Metallic.png");
		c->getStaticMesh()->getMaterial(0).emissiveTexture = da::graphics::CTexture2DFactory::Create("assets/sniper/Textures/Shared/Sniper_Emission.png");
		c->getStaticMesh()->getMaterial(0).emissiveFactor = { 1.f,1.f,1.f };
		c->getStaticMesh()->getMaterial(0).occlusionTexture = da::graphics::CTexture2DFactory::Create("assets/sniper/Textures/Shared/Sniper_Occlusion.png");
		
		e3 = da::core::CSceneManager::getScene()->createEntity();
		c = e3->addComponent<da::core::CSmeshComponent>("assets/rifle/Rifle.fbx");
		c->getStaticMesh()->getMaterial(0).baseColorTexture = da::graphics::CTexture2DFactory::Create("assets/rifle/Textures/Albedo.png");
		c->getStaticMesh()->getMaterial(0).normalTexture = da::graphics::CTexture2DFactory::Create("assets/rifle/Textures/Normal.png");
		c->getStaticMesh()->getMaterial(0).metallicRoughnessTexture = da::graphics::CTexture2DFactory::Create("assets/rifle/Textures/Metallic.png");
		c->getStaticMesh()->getMaterial(0).emissiveTexture = da::graphics::CTexture2DFactory::Create("assets/rifle/Textures/Emission.png");
		c->getStaticMesh()->getMaterial(0).emissiveFactor = { 1.f,1.f,1.f };
		c->getStaticMesh()->getMaterial(0).occlusionTexture = da::graphics::CTexture2DFactory::Create("assets/rifle/Textures/Occlusion.png");

		e3->setTag(HASHSTR("Rifle"));
		e3->getTransform().setPosition({ 0,-5.f,25.f });
		e3->getTransform().setRotation({ 0,0.f,90.f });
		da::core::CCamera::getCamera()->setPosition({ 0,0,1 });

		e5 = da::core::CSceneManager::getScene()->createEntity();
		c = e5->addComponent<da::core::CSmeshComponent>("assets/blade_LOTR/blade_LOTR.fbx");
		c->getStaticMesh()->getMaterial(0).baseColorTexture = da::graphics::CTexture2DFactory::Create("assets/blade_LOTR/blade_LOTR_phong4_BaseColor.png");
		c->getStaticMesh()->getMaterial(0).normalTexture = da::graphics::CTexture2DFactory::Create("assets/blade_LOTR/blade_LOTR_phong4_Normal_OpenGL.png");
		c->getStaticMesh()->getMaterial(0).occlusionTexture = da::graphics::CTexture2DFactory::Create("assets/blade_LOTR/blade_LOTR_phong4_AmbientOcclusion_Mixed.png");

		e5->setTag(HASHSTR("hat"));
		e5->getTransform().setPosition({ 0,0,0 });
		e5->getTransform().setRotation({ 45.f,0.f,180.f });
		e5->getTransform().setScale({ 4.5f,4.5f,4.5f });
		da::core::CCamera::getCamera()->setPosition({ 0,0,1 });

		da::platform::CBgfxSkeletalMesh* mesh = new da::platform::CBgfxSkeletalMesh("assets/mannequin/zombie.fbx", false);
		mesh->getMaterial(0).baseColorTexture = da::graphics::CTexture2DFactory::Create("assets/mannequin/alpha_body_mat.png");
		mesh->getMaterial(0).metallicFactor = .1500f;
		mesh->getMaterial(0).roughnessFactor = 0.f;
		mesh->getMaterial(1).baseColorFactor = { .45f,0.45f,0.45f,1.f };
		da::graphics::CSkeletalAnimation* animation = new da::graphics::CSkeletalAnimation("assets/mannequin/SwordSlash.fbx", mesh);
		da::graphics::CSkeletalAnimator* animator = new da::graphics::CSkeletalAnimator(animation);

		for (int i = 0; i < 1; i++) {
			e4 = da::core::CSceneManager::getScene()->createEntity();
			da::core::FComponentRef<da::core::CSkeletalMeshComponent> cc = e4->addComponent<da::core::CSkeletalMeshComponent>(mesh, animation, animator);
			//cc->getSkeletalMesh()->getMaterial(0).baseColorFactor = { 0.0f,0.0f,0.8f,1.f };
			
			e4->getTransform().setPosition({ 0,5, -.6 });
			e4->getTransform().setRotation({ 90.f,0.f,0.f });

			//e4->getTransform().setPosition({ i*3.f,5.f+(1.5f*i),-.1f});
			//e4->getTransform().setRotation({ 90.f,180.f,0.f });
		}

		da::core::CCamera::getCamera()->setPosition({ 0,0,1 });

		e2->getTransform().setPosition({ 0,-1,0 });
		e2->getTransform().setRotation({ 0,0,160 });
		e2->getTransform().setScale({ 5,5,5});
		e2->setTag(HASHSTR("plane"));
		c = e2->addComponent<da::core::CSmeshComponent>("assets/city/city2.fbx");
		c->getStaticMesh()->getMaterial(0).baseColorTexture = da::graphics::CTexture2DFactory::Create("assets/city/wallTextures/wallA.jpg");
		c->getStaticMesh()->getMaterial(0).normalTexture = da::graphics::CTexture2DFactory::Create("assets/city/wallTextures/wallN.jpg");
		c->getStaticMesh()->getMaterial(0).metallicRoughnessTexture = da::graphics::CTexture2DFactory::Create("assets/city/wallTextures/wallR.jpg");
		c->getStaticMesh()->getMaterial(0).metallicFactor = .25f;
		c->getStaticMesh()->getMaterial(0).roughnessFactor = 1.f;
		c->getStaticMesh()->getMaterial(0).doubleSided = true;

		c->getStaticMesh()->getMaterial(1).baseColorTexture = da::graphics::CTexture2DFactory::Create("assets/marble/MarbleA.jpg");
		c->getStaticMesh()->getMaterial(1).normalTexture = da::graphics::CTexture2DFactory::Create("assets/marble/MarbleN.jpg");
		c->getStaticMesh()->getMaterial(1).metallicRoughnessTexture = da::graphics::CTexture2DFactory::Create("assets/marble/MarbleR.jpg");
		c->getStaticMesh()->getMaterial(1).uvScale = { 11.f,11.f };
		c->getStaticMesh()->getMaterial(1).doubleSided = true;
		return;
	
		
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
#ifdef DA_DEBUG
		da::debug::CDebugMenuBar::unregister_debug(HASHSTR("Script"), HASHSTR("Reload Scripts"));
#endif
		da::CLogger::LogDebug(da::ELogChannel::Application, "App End");
	}

	inline void renderScriptDebug(bool soft, bool* b) {

		auto& components = da::core::CSceneManager::getScene()->getComponents<da::core::CScriptComponent>();
		da::script::CScriptEngine::clearAll();
		for (size_t i = 0; i < components.getCount(); i++) {
			da::core::CScriptComponent* c = (da::core::CScriptComponent*)components.getComponentAtIndex(i);
			c->reload(soft);
		}
		*b = false;
		
	}

	glm::vec3 rotOffset = { 80.f,185.f,84.f };

	inline virtual void onUpdate(float dt) override
	{
		if (dt > 1.0) return;

		float moveSpeed = 0;// 12.5f;

		glm::vec3 pos = e4->getTransform().position();

		pos.y += moveSpeed * dt;

		e4->getTransform().setPosition(pos);

		da::core::FComponentRef<da::core::CSkeletalMeshComponent> component = e4->getComponent<da::core::CSkeletalMeshComponent>();

		glm::mat4 worldBoneTransform;
		if (component->getSkeletalAnimator()->getBoneWorldTransform(HASHSTR("mixamorig_RightHand"), e4->getTransform().matrix(), worldBoneTransform)) {
			worldBoneTransform = glm::translate(worldBoneTransform, glm::vec3(0.f, -3.f, -9.f));
			worldBoneTransform = worldBoneTransform * glm::toMat4(glm::quat(glm::radians(rotOffset)));

			glm::vec3 scale;
			glm::quat rotation;
			glm::vec3 translation;
			glm::vec3 skew;
			glm::vec4 perspective;
			glm::decompose(worldBoneTransform, scale, rotation, translation, skew, perspective);

			e5->getTransform().setPosition(translation);
			e5->getTransform().setRotation(rotation);
		}


		if (ImGui::Begin("WODKAD"))
		{
			ImGui::DragFloat3("rot", &rotOffset.x, 1.f, -360, 360);
		}

		ImGui::End();

		glm::quat worldBoneRot;
		if (component->getSkeletalAnimator()->getBoneWorldRotation(HASHSTR("mixamorig_RightHand"), e4->getTransform().matrix(), worldBoneRot)) {
			//e5->getTransform().setRotation(worldBoneRot + rotOffset);

		}

	
	}
};
