#include "game.h"

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


#ifdef DA_REVIEW
#include <DAEngine/debug/debug_menu_bar.h>
#endif
#include <DAEngine/core/ecs/skeletal_mesh_component.h>
#include <DAEngine/core/time.h>
#include <glm/gtx/quaternion.hpp>
#include <DAEngine/core/graphics/graphics_debug_render.h>
#include <DAEngine/physics/physics.h>
#include <DAEngine/physics/physics_shape.h>
#include <DAEngine/physics/physics_rigid_body.h>
#include <DAEngine/core/ecs/rigid_body_component.h>

#include "character/character.h"


void CGame::createModules()
{
	m_window = new da::modules::CWindowModule({ "game", 1280, 720, 0,0, 144, da::core::EWindowDeco::NONE });
	addModule(m_window);

	m_window->getEventHandler().registerCallback(EEventCategory::Window, BIND_EVENT_FN(CGame, windowEvent));

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

void CGame::windowEvent(const da::core::events::CEvent& e)
{

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

void CGame::onInitialize()
{
	createModules();
#ifdef DA_REVIEW
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
	e1->getTransform().setPosition({ 0,0,25.5f });
	
	da::core::CCamera::getCamera()->setPosition({ 0,0,1 });
	/*
	e5 = da::core::CSceneManager::getScene()->createEntity();
	da::core::FComponentRef<da::core::CSmeshComponent> c = e5->addComponent<da::core::CSmeshComponent>("assets/blade_LOTR/blade_LOTR.fbx");
	c->getStaticMesh()->getMaterial(0).setBaseColorTexture(da::graphics::CTexture2DFactory::Create("assets/blade_LOTR/blade_LOTR_phong4_BaseColor.png"));
	c->getStaticMesh()->getMaterial(0).setNormalTexture(da::graphics::CTexture2DFactory::Create("assets/blade_LOTR/blade_LOTR_phong4_Normal_OpenGL.png"));
	c->getStaticMesh()->getMaterial(0).setOcclusionTexture(da::graphics::CTexture2DFactory::Create("assets/blade_LOTR/blade_LOTR_phong4_AmbientOcclusion_Mixed.png"));

	e5->setTag(HASHSTR("sw0rd"));
	e5->getTransform().setPosition({ 0,0,0 });
	e5->getTransform().setRotation({ 45.f,0.f,180.f });
	e5->getTransform().setScale({ 1.f,1.f,1.f });
	da::core::CCamera::getCamera()->setPosition({ 0,0,1 });

	
	e6 = da::core::CSceneManager::getScene()->createEntity();
	c = e6->addComponent<da::core::CSmeshComponent>("assets/hat/hat.fbx");
	c->getStaticMesh()->getMaterial(0).setBaseColorTexture(da::graphics::CTexture2DFactory::Create("assets/hat/Hat_albedo.jpg"));
	c->getStaticMesh()->getMaterial(0).setNormalTexture(da::graphics::CTexture2DFactory::Create("assets/hat/Hat_normal.png"));
	c->getStaticMesh()->getMaterial(0).setOcclusionTexture(da::graphics::CTexture2DFactory::Create("assets/hat/Hat_AO.jpg"));
	c->getStaticMesh()->getMaterial(0).roughnessFactor = 1.f;
	c->getStaticMesh()->getMaterial(0).metallicFactor = 0.f;

	e6->setTag(HASHSTR("hat"));
	e6->getTransform().setPosition({ 0,0,0 });
	e6->getTransform().setRotation({ 45.f,0.f,180.f });
	e6->getTransform().setScale({ .1f,.1f,.1f });
	*/
	da::core::CCamera::getCamera()->setPosition({ 0,0,1 });

	e7 = da::core::CSceneManager::getScene()->createEntity();
	da::core::FComponentRef<da::core::CSmeshComponent> c = e7->addComponent<da::core::CSmeshComponent>("assets/sphere.fbx");
	e7->getTransform().setPosition({0.f,38.f, 5.f});
	e7->addComponent<da::core::CRigidBodyComponent>(
		da::physics::IPhysicsRigidBody::create(da::physics::CPhysicsShapeSphere::create(1.f)
			, da::physics::CPhysicsEntityMotionState::create(e7)
			, 1.f
			, { 0.f,0.f,0.f }));
	//e7->addComponent<da::core::CRigidBodyComponent>(da::physics::IPhysicsRigidBody::create(da::physics::CPhysicsShapeSphere::create(1.f), da::physics::CPhysicsDefaultMotionState::create(e7->getTransform().matrix()), 1.f, { 0.f,0.f,0.f }));
	//c->getStaticMesh()->getMaterial(0).setBaseColorTexture(da::graphics::CTexture2DFactory::Create("assets/debugTexture.jpeg"));
	//c->getStaticMesh()->getMaterial(0).roughnessFactor = 0.f;
	//c->getStaticMesh()->getMaterial(0).metallicFactor = .5f;
	e7->setTag(HASHSTR("cube"));

	da::platform::CBgfxSkeletalMesh* mesh = new da::platform::CBgfxSkeletalMesh("assets/mannequin/SwordSlash.fbx", false);
	mesh->getMaterial(0).setBaseColorTexture(da::graphics::CTexture2DFactory::Create("assets/mannequin/alpha_body_mat.png"));
	mesh->getMaterial(0).setNormalTexture(nullptr);
	mesh->getMaterial(0).metallicFactor = .1500f;
	mesh->getMaterial(0).roughnessFactor = 0.f;
	mesh->getMaterial(1).baseColorFactor = { .45f,0.45f,0.45f,1.f };
	m_swordSlashAnimation = new da::graphics::CSkeletalAnimation("assets/mannequin/SwordSlash.fbx", mesh);
	da::graphics::CSkeletalAnimator* animator = new da::graphics::CSkeletalAnimator(m_swordSlashAnimation);

	da::graphics::CSkeletalAnimation* animation = m_swordSlashAnimation;

	for (int i = 1; i < 2; i++) {
		break;
		if (i == 1) {
			m_runAnimation = new da::graphics::CSkeletalAnimation("assets/mannequin/SwordRun.fbx", mesh);
			animation = m_runAnimation;
			animator = new da::graphics::CSkeletalAnimator(animation);
		}

		e4 = da::core::CSceneManager::getScene()->createEntity();
		e4->setTag(HASHSTR("Mannequin"));
		da::core::FComponentRef<da::core::CSkeletalMeshComponent> cc = e4->addComponent<da::core::CSkeletalMeshComponent>(mesh, animation, animator);
		e4->addComponent<da::core::CScriptComponent>("scripts/build/camera_component.lua", "CameraComponent");
		
		//cc->getSkeletalMesh()->getMaterial(0).baseColorFactor = { 0.0f,0.0f,0.8f,1.f };

		e4->getTransform().setPosition({ 0,5 + (i * -5.f), 5.f });
		e4->getTransform().setRotation({ 0.f,0.f,180.f });
		

		glm::mat4 offset = glm::translate(glm::mat4(1.f), { 0.f,0.f, -1.15f }) * glm::toMat4(glm::quat(glm::radians(glm::vec3(90.f, 0.f, 0.f))));

		cc->setTransform(offset);

		da::core::FComponentRef<da::core::CRigidBodyComponent> rb = e4->addComponent<da::core::CRigidBodyComponent>(
			da::physics::IPhysicsRigidBody::create(da::physics::CPhysicsShapeCapsule::create(.3f, 1.7f)
				, da::physics::CPhysicsEntityMotionState::create(e4)
				, 1.f
				, { 0.f,0.f,0.f }));


		rb->getPhysicsBody()->setAngularFactor({ 0.f,0.f,0.f });
		//e4->getTransform().setPosition({ i*3.f,5.f+(1.5f*i),-.1f});
		//e4->getTransform().setRotation({ 90.f,180.f,0.f });
	}

	da::core::CCamera::getCamera()->setPosition({ 0,0,1 });

	e2->getTransform().setPosition({ 0,0,0 });
	e2->getTransform().setRotation({ 0,0,0 });

	c = e2->addComponent<da::core::CSmeshComponent>("assets/floor.fbx");
	e2->addComponent<da::core::CRigidBodyComponent>(da::physics::IPhysicsRigidBody::create(da::physics::CPhysicsShapeCube::create({100.f,100.f,0.1f}), da::physics::CPhysicsDefaultMotionState::create(e2->getTransform().matrix()), 0.f, { 0.f,0.f,0.f }));

	//e2->getTransform().setScale({ 100,100,1.f });
	e2->setTag(HASHSTR("plane"));
	//c = e2->addComponent<da::core::CSmeshComponent>("assets/terrain/terrain1.fbx");
	//e2->addComponent<da::core::CRigidBodyComponent>(da::physics::IPhysicsRigidBody::create(da::physics::CPhysicsShapeTriangleMesh::create(c->getStaticMesh()), da::physics::CPhysicsDefaultMotionState::create(e2->getTransform().matrix()), 0.f, { 0.f,0.f,0.f }));
	
	c->getStaticMesh()->getMaterial(0).setBaseColorTexture(da::graphics::CTexture2DFactory::Create("assets/textures/tex_debug_grid_02.png"));
	c->getStaticMesh()->getMaterial(0).uvScale = { 11.f,11.f };
	c->getStaticMesh()->getMaterial(0).doubleSided = false;
	c->getStaticMesh()->getMaterial(0).metallicFactor = 0.0;
	c->getStaticMesh()->getMaterial(0).roughnessFactor = 1.0;

	e2 = da::core::CSceneManager::getScene()->createEntity();

	e2->getTransform().setPosition({ 0,30,-3 });
	e2->getTransform().setRotation({ 15,0,0 });

	c = e2->addComponent<da::core::CSmeshComponent>("assets/cube.fbx");
	e2->addComponent<da::core::CRigidBodyComponent>(da::physics::IPhysicsRigidBody::create(da::physics::CPhysicsShapeCube::create({ 5.f,10.f,5.f }), da::physics::CPhysicsDefaultMotionState::create(e2->getTransform().matrix()), 0.f, { 0.f,0.f,0.f }));

	e2->getTransform().setScale({ 5,10,5.f });
	e2->setTag(HASHSTR("ramp1"));

	c->getStaticMesh()->getMaterial(0).setBaseColorTexture(da::graphics::CTexture2DFactory::Create("assets/debugTexture.jpeg"));
	//c->getStaticMesh()->getMaterial(0).setNormalTexture(da::graphics::CTexture2DFactory::Create("assets/marble/MarbleN.jpg"));
	//c->getStaticMesh()->getMaterial(0).setMetallicRoughnessTexture(da::graphics::CTexture2DFactory::Create("assets/marble/MarbleR.jpg"));
	c->getStaticMesh()->getMaterial(0).uvScale = { 11.f,11.f };
	c->getStaticMesh()->getMaterial(0).doubleSided = false;

	m_character = new CCharacter();
	m_character->initialize();

	m_vehicle = new CVehicle();
	m_vehicle->initialize();

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

void CGame::onUpdate(float dt)
{
	m_character->update(dt);
	m_vehicle->update(dt);
	return;
	if (dt > 1.0) return;

	float moveSpeed = 0.f;//1.5f;

	glm::vec3 pos = e4->getTransform().position();

	pos.y += moveSpeed * dt;

	e4->getTransform().setPosition(pos);

	da::core::FComponentRef<da::core::CSkeletalMeshComponent> component = e4->getComponent<da::core::CSkeletalMeshComponent>();

	glm::mat4 worldBoneTransform;


	if (component->getSkeletalAnimator()->getBoneWorldTransform(HASHSTR("mixamorig_RightHand"), component->getTransform(), worldBoneTransform)) {
		worldBoneTransform = glm::translate(worldBoneTransform, glm::vec3(0.f, -3.f, 10.f));
		worldBoneTransform = worldBoneTransform * glm::toMat4(glm::quat(glm::radians(glm::vec3(-90.f, 185.f, 90.f))));

		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(worldBoneTransform, scale, rotation, translation, skew, perspective);

		e5->getTransform().setPosition(translation);
		e5->getTransform().setRotation(rotation);
	}

#ifdef DA_REVIEW
	component->getSkeletalAnimator()->debugRenderJoints(component->getTransform());
	//da::graphics::CDebugRender::getInstance()->drawLine({ 0.f, 0.f, -10.f }, { 0.f,0.f, 10.f }, 1.f, { 1.f,0.f,0.f,1.f });
#endif
	



	if (component->getSkeletalAnimator()->getBoneWorldTransform(HASHSTR("mixamorig_Head"), component->getTransform(), worldBoneTransform)) {
		worldBoneTransform = glm::translate(worldBoneTransform, glm::vec3(0.f, 18.f, 0.f));
		worldBoneTransform = worldBoneTransform * glm::toMat4(glm::quat(glm::radians(glm::vec3(80.f, -180.f, 0.f))));

		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(worldBoneTransform, scale, rotation, translation, skew, perspective);

		e6->getTransform().setPosition(translation);
		e6->getTransform().setRotation(rotation);
	}
	
	da::physics::FRayData data(da::physics::ERayType::All, { 0.f,0.f, 10.f }, { 0.f, 0.f, -10.f });
	da::physics::CPhysics::rayCast(data);

	da::core::FComponentRef<da::core::CRigidBodyComponent> rb = e4->getComponent<da::core::CRigidBodyComponent>();

	glm::vec3 cPos = e4->getTransform().position();
	glm::vec3 cOffset = (e4->getTransform().up() * .5f) + e4->getTransform().forward()*-2.f;

	da::core::CCamera::getCamera()->setPosition(cPos+cOffset);
	//da::core::CCamera::getCamera()->lookAt(cPos + e4->getTransform().up()*.25f);

#ifdef DA_REVIEW
	if (ImGui::Begin("Hit?")) {
		if (data.bHit) {
			for (int i = 0; i < data.vHits.size(); i++) {
				ImGui::Text("HIT: %s", data.vHits[i].pEntity->getTag().c_str());
			}
		}
		else
		{
			ImGui::Text("NO HIT");
		}

		if (ImGui::Button("Swing sword anim")) {
			component->getSkeletalAnimator()->playAnimation(m_swordSlashAnimation, .25f);
		}
		if (ImGui::Button("Run Anim")) {
			component->getSkeletalAnimator()->playAnimation(m_runAnimation, .15f);
		}
	}

	ImGui::End();
#endif

}

void CGame::onShutdown()
{
	m_character->shutdown();
	delete m_character;

	m_vehicle->shutdown();
	delete m_vehicle;
#ifdef DA_REVIEW
	da::debug::CDebugMenuBar::unregister_debug(HASHSTR("Script"), HASHSTR("Reload Scripts"));
#endif
	da::CLogger::LogDebug(da::ELogChannel::Application, "App End");
}

void CGame::renderScriptDebug(bool soft, bool* b)
{

	auto& components = da::core::CSceneManager::getScene()->getComponents<da::core::CScriptComponent>();
	da::script::CScriptEngine::clearAll();
	for (size_t i = 0; i < components.getCount(); i++) {
		da::core::CScriptComponent* c = (da::core::CScriptComponent*)components.getComponentAtIndex(i);
		c->reload(soft);
	}
	*b = false;

}