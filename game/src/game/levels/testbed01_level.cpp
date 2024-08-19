#include "testbed01_level.h"

//engine
#include <daengine/ai.h>
#include <daengine/graphics.h>
#include <daengine/core.h>
#include <daengine/physics.h>

// game
#include "game/character/character.h"
#include "game/vehicle/vehicle.h"
#include <game/vehicle/vehicle_manager.h>
#include "game/helpers/texture_helper.h"


glm::vec4 hexToVec(uint32_t i) {
	// Extract individual color components from the hexadecimal code
	uint8_t r = (i >> 24) & 0xFF;
	uint8_t g = (i >> 16) & 0xFF;
	uint8_t b = (i >> 8) & 0xFF;
	uint8_t a = i & 0xFF;

	// Normalize the color components to the range [0, 1]
	float rf = static_cast<float>(r) / 255.0f;
	float gf = static_cast<float>(g) / 255.0f;
	float bf = static_cast<float>(b) / 255.0f;
	float af = static_cast<float>(a) / 255.0f;

	// Return the color as a glm::vec4
	return glm::vec4(rf, gf, bf, af);
}

CTestBed01Level::CTestBed01Level(CHashString name, da::modules::CGraphicsModule& graphics, da::modules::CWindowModule& window) : ILevel(name, graphics, window), m_scrlevel("scripts/build/levels/test_bed_01.lua", "TestBed01", "main", false)
{

}

void CTestBed01Level::initialize()
{
	m_windowModule.getWindow()->lockCursor(true);
	m_scrlevel.setup({}, {});
	da::core::CSceneManager::setScene(new da::core::CScene(da::core::CGuid("2f94546a-a92a-4046-946a-f28c2364110b")));
	da::core::CCamera::getCamera()->setPosition({ 0,0,1 });
	da::ai::CTiledNavMesh* navMesh = new da::ai::CTiledNavMesh();

	// Test Bed
	{
		da::core::CEntity* testBed = da::core::CSceneManager::getScene()->createEntity();
		da::graphics::CStaticMesh* colMesh = da::factory::CStaticMeshFactory::create("assets/prop/level/dungeon_test_col.fbx");
		da::core::FComponentRef<da::core::CSmeshComponent> meshComponent = testBed->addComponent<da::core::CSmeshComponent>("assets/prop/level/dungeon_test.fbx");

		for (int i = 0; i < meshComponent->getStaticMesh()->getMaterialCount(); i++) {
			meshComponent->getStaticMesh()->getMaterial(i).metallicFactor = 0.0;
			meshComponent->getStaticMesh()->getMaterial(i).roughnessFactor = 1.0;
			meshComponent->getStaticMesh()->getMaterial(i).doubleSided = true;
		}

		meshComponent->getStaticMesh()->castShadows(true);

		std::vector<da::physics::IPhysicsShape*> shapes;
		std::vector<glm::mat4> transforms;

		for (int i = 0; i < colMesh->getMeshes().size(); i++) {
			shapes.push_back(da::physics::CPhysicsShapeTriangleMesh::create(colMesh, i));
			transforms.push_back(testBed->getTransform().matrix());
		}

		da::physics::CPhysicsShapeCompound* compoundCollision = da::physics::CPhysicsShapeCompound::create(shapes, transforms);

		testBed->addComponent<da::core::CRigidBodyComponent>(
			da::physics::IPhysicsRigidBody::create(
				compoundCollision
				, da::physics::CPhysicsDefaultMotionState::create(testBed->getTransform().matrix())
				, 0.f
				, { 0.f,0.f,0.f }));
		testBed->setTag(HASHSTR("TestBed"));
		testBed->getTransform().setScale({ 1.f, 1.f, 1.f });

		// Nav mesh
		navMesh->addMesh(testBed->getTransform().matrix(), colMesh->getMeshes()[0]);
	}

	// Character
	m_character = new CCharacter(*m_graphicsModule.getGraphicsApi(), true);
	m_character->initialize();
	m_ai = new CCharacter(*m_graphicsModule.getGraphicsApi(), false);
	m_ai ->initialize();

	// Hello World
	{
		da::core::CEntity* helloWorldEntity = da::core::CSceneManager::getScene()->createEntity();
		helloWorldEntity->setTag(HASHSTR("helloworld"));
		helloWorldEntity->addComponent<da::core::CScriptComponent>("scripts/build/helloworld.lua", "MyComponent");
	}

	// Ramp1
	{
		da::core::CEntity* ramp = da::core::CSceneManager::getScene()->createEntity();
		ramp->getTransform().setPosition({ 0,30,-3 });
		ramp->getTransform().setRotation({ 15,0,0 });
		ramp->getTransform().setScale({ 5,10,5.f });
		ramp->setTag(HASHSTR("ramp1"));

		da::core::FComponentRef<da::core::CSmeshComponent> meshComponent = ramp->addComponent<da::core::CSmeshComponent>("assets/cube.fbx");
		meshComponent->getStaticMesh()->getMaterial(0).setBaseColorTexture(CTextureHelper::create(Texture::tex_debug_grid_01));

		ramp->addComponent<da::core::CRigidBodyComponent>(
			da::physics::IPhysicsRigidBody::create(da::physics::CPhysicsShapeCube::create({ 5.f,10.f,5.f })
				, da::physics::CPhysicsDefaultMotionState::create(ramp->getTransform().matrix())
				, 0.f
				, { 0.f,0.f,0.f }));
	}

	// Dummy obj
	{
		da::core::CEntity* transformObj = da::core::CSceneManager::getScene()->createEntity();
		transformObj->getTransform().setPosition({ -10.000,-13.000,6.000 });
		transformObj->getTransform().setRotation({ 0,0,0 });
		transformObj->setTag(HASHSTR("dummy"));

		da::core::FComponentRef<da::core::CSmeshComponent> mesh = transformObj->addComponent<da::core::CSmeshComponent>("assets/prop/misc/target_dummy.fbx");
		mesh->getStaticMesh()->getMaterial(0).setBaseColorTexture(CTextureHelper::create(Texture::dummy_baseColor));
		mesh->getStaticMesh()->getMaterial(0).setNormalTexture(CTextureHelper::create(Texture::dummy_normal));
		mesh->getStaticMesh()->getMaterial(0).metallicFactor = 0.f;
		mesh->getStaticMesh()->getMaterial(0).roughnessFactor = 1.f;

		da::graphics::CStaticMesh* colMesh = da::factory::CStaticMeshFactory::create("assets/prop/misc/target_dummy_collision.fbx", false);

		transformObj->addComponent<da::core::CRigidBodyComponent>(
			da::physics::IPhysicsRigidBody::create(da::physics::CPhysicsShapeTriangleMesh::create(colMesh)
				, da::physics::CPhysicsDefaultMotionState::create(transformObj->getTransform().matrix())
				, 0.f
				, { 0.f,0.f,0.f }));
	}

	{
		da::core::CEntity* arena = da::core::CSceneManager::getScene()->createEntity();
		arena->getTransform().setPosition({ 0,0,0.f });
		arena->getTransform().setRotation({ 0,0,0 });
		arena->setTag(HASHSTR("arena"));

		da::core::FComponentRef<da::core::CSmeshComponent> meshComponent = arena->addComponent<da::core::CSmeshComponent>("assets/prop/structure/prop_gladiator_arena.fbx");
		meshComponent->getStaticMesh()->castShadows(false);

		da::graphics::CStaticMesh* collision = da::factory::CStaticMeshFactory::create("assets/prop/structure/prop_collision_gladiator_arena.fbx");
		/*arena->addComponent<da::core::CRigidBodyComponent>(da::physics::IPhysicsRigidBody::create(
			da::physics::CPhysicsShapeTriangleMesh::create(collision)
			, da::physics::CPhysicsDefaultMotionState::create(arena->getTransform().matrix())
			, 0.f
			, { 0.f,0.f,0.f }));
*/

		meshComponent->getStaticMesh()->getMaterial(0).baseColorFactor = hexToVec(0xc29d84ff);
		meshComponent->getStaticMesh()->getMaterial(1).baseColorFactor = hexToVec(0xBBBCBCFF);
		meshComponent->getStaticMesh()->getMaterial(2).baseColorFactor = hexToVec(0xBBB1A8FF);
		meshComponent->getStaticMesh()->getMaterial(3).baseColorFactor = hexToVec(0xBBAD46FF);
		meshComponent->getStaticMesh()->getMaterial(4).baseColorFactor = hexToVec(0x7C5430FF);
		meshComponent->getStaticMesh()->getMaterial(5).baseColorFactor = hexToVec(0x832D2Dff);

		for (size_t i = 0; i < 6; i++) {
			meshComponent->getStaticMesh()->getMaterial(i).metallicFactor = 0.5;
			meshComponent->getStaticMesh()->getMaterial(i).roughnessFactor = .750;
			meshComponent->getStaticMesh()->getMaterial(i).doubleSided = false;
		}
	}

	m_scrlevel.classInitialize();

	m_windowModule.getWindow()->getEventHandler().registerCallback(EEventType::InputKeyboard, BIND_EVENT_FN(CTestBed01Level, onKeyboardEvent));
	da::ai::CNavMeshManager::addNavMesh(*navMesh);
	// create vehicle
	//createVehicle();
}

void CTestBed01Level::onKeyboardEvent(const da::core::events::CEvent& event)
{
	const da::core::events::CInputKeyboardEvent& kb = (da::core::events::CInputKeyboardEvent&)event;

	if (kb.getBtn() != 290 || kb.getType() != EInputType::RELEASED) // F1
	{
		return;
	}

	if (!m_freeCam)
	{
		da::core::CInput::pushInputContext(HASHSTR("FreeCam"), 1e9);
	}
	else
	{
		da::core::CInput::popInputContext(HASHSTR("FreeCam"));
	}
	m_freeCam = !m_freeCam;
	m_windowModule.getWindow()->lockCursor(!m_freeCam);
}

void CTestBed01Level::update(float dt)
{
	// R
	if (da::core::CInput::inputPressed(82)) {
		auto& components = da::core::CSceneManager::getScene()->getComponents<da::core::CScriptComponent>();
		da::script::CScriptEngine::clearAll();
		for (size_t i = 0; i < components.getCount(); i++) {
			da::core::CScriptComponent* c = (da::core::CScriptComponent*)components.getComponentAtIndex(i);
			c->reload(false);
		}
		m_scrlevel.classShutdown();
		m_scrlevel.cleanup();
		m_scrlevel.setup({}, {});
		m_scrlevel.classInitialize();
		//destroyVehicle();
		//createVehicle();
	}

	m_character->update(dt);
	m_ai->update(dt);
	//m_vehicle->update(dt);

	m_scrlevel.classUpdate(dt);
}

void CTestBed01Level::lateUpdate(float dt)
{
	m_character->lateUpdate(dt);
	m_ai->lateUpdate(dt);

	if (m_freeCam)
	{
		static glm::vec2 cursorPos = { 0.f, 0.f };
		const float camSpeed = 5.f;

		if (!m_camera)
		{
			m_camera = new da::core::CCamera();
		}


		da::core::CCamera* cam = m_camera;

		if (da::core::CCamera* oldCam = da::core::CCamera::getCamera())
		{
			if (oldCam != cam)
			{
				da::core::CCamera::setCamera(cam);
				cam->setPosition(oldCam->position());
				cam->setRotation(oldCam->rotation());
			}
		}

		

		if (da::core::CInput::inputPressed(87)) // W
		{
			cam->offsetPosition(cam->forward() * camSpeed * dt);
		}

		if (da::core::CInput::inputPressed(83)) // S
		{
			cam->offsetPosition(-cam->forward() * camSpeed * dt);
		}

		if (da::core::CInput::inputPressed(65)) // A
		{
			cam->offsetPosition(-cam->right() * camSpeed * dt);
		}

		if (da::core::CInput::inputPressed(68)) // D
		{
			cam->offsetPosition(cam->right() * camSpeed * dt);
		}

		if (da::core::CInput::inputPressed(69)) // E
		{
			cam->offsetPosition(-cam->up() * camSpeed * dt);
		}

		if (da::core::CInput::inputPressed(81)) // Q
		{
			cam->offsetPosition(cam->up() * camSpeed * dt);
		}

		glm::vec2 pos = { da::core::CInput::getCursorX(), da::core::CInput::getCursorY() };
		if (da::core::CInput::mouseInputPressed(1))
		{
			if (cursorPos.x >= 0.0 && cursorPos.y >= 0.0)
			{
				cam->offsetRotation({ cursorPos.y - pos.y, 0.f, cursorPos.x - pos.x });
			}
		}
		cursorPos = pos;
	}

}

void CTestBed01Level::shutdown()
{

	m_windowModule.getWindow()->getEventHandler().unregisterCallback(EEventType::InputKeyboard, BIND_EVENT_FN(CTestBed01Level, onKeyboardEvent));

	if (m_freeCam)
	{
		da::core::CInput::popInputContext(HASHSTR("FreeCam"));
		m_windowModule.getWindow()->lockCursor(false);
		m_freeCam = false;
	}

	m_scrlevel.classShutdown();
	m_scrlevel.cleanup();

	m_ai->shutdown();
	delete m_ai;

	m_character->shutdown();
	delete m_character;

	delete m_camera;

	da::core::CSceneManager::setScene(nullptr);
}

void CTestBed01Level::createVehicle()
{
	// Vehicle
	m_vehicle = new CVehicle(m_graphicsModule);
	m_vehicle->initialize(const_cast<da::modules::CWindowModule*>(&m_windowModule), CVehicleManager::getVehicleTypes().begin()->second);
}

void CTestBed01Level::destroyVehicle()
{
	m_vehicle->shutdown();
	delete m_vehicle;
}


