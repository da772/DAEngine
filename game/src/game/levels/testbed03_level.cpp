#include "testbed03_level.h"

//engine
#include <daengine/graphics.h>
#include <daengine/physics.h>
#include <daengine/components.h>


// game
#include "game/character/character.h"
#include "game/vehicle/vehicle.h"
#include <game/vehicle/vehicle_manager.h>


CTestBed03Level::CTestBed03Level(CHashString name, da::modules::CGraphicsModule& graphics, da::modules::CWindowModule& window) 
	: ILevel(name,graphics, window)
	, m_window(window)
	, m_scrlevel("scripts/build/levels/test_bed_01.lua", "TestBed01", "main", false)
{
	FLevelGenSettings settings;
	settings.Width = 100;
	settings.Height = 100;
	settings.Nodes = 15;
	settings.Seed = 2556;
	settings.StartingNode = { -100, -100 };
	m_levelGen = CLevelGenerator(settings);
}

void CTestBed03Level::initialize()
{
	m_scrlevel.setup({}, {});
	da::core::CSceneManager::setScene(new da::core::CScene(da::core::CGuid("2f94546a-a92a-4046-946a-f28c2364110b")));
	da::core::CCamera::getCamera()->setPosition({ 0,0,1 });

	// Character
	m_character = new CCharacter();
	m_character->initialize();

	// Hello World
	{
		da::core::CEntity* helloWorldEntity = da::core::CSceneManager::getScene()->createEntity();
		helloWorldEntity->setTag(HASHSTR("helloworld"));
		helloWorldEntity->addComponent<da::core::CScriptComponent>("scripts/build/helloworld.lua", "MyComponent");
	}

	// Sphere
	{
		da::core::CEntity* sphere = da::core::CSceneManager::getScene()->createEntity();
		da::core::FComponentRef<da::core::CSmeshComponent> c = sphere->addComponent<da::core::CSmeshComponent>("assets/sphere.fbx");
		sphere->addComponent<da::core::CRigidBodyComponent>(
			da::physics::IPhysicsRigidBody::create(da::physics::CPhysicsShapeSphere::create(1.f)
				, da::physics::CPhysicsEntityMotionState::create(sphere)
				, 1.f
				, { 0.f,0.f,0.f }));
		sphere->setTag(HASHSTR("Sphere"));
	}

	// Test Bed
	{
		da::core::CEntity* testBed = da::core::CSceneManager::getScene()->createEntity();
		da::core::FComponentRef<da::core::CSmeshComponent> meshComponent = testBed->addComponent<da::core::CSmeshComponent>("assets/plane100.fbx");
		meshComponent->getStaticMesh()->getMaterial(0).setBaseColorTexture(da::graphics::CTexture2DFactory::Create("assets/textures/surface/road/Tex_Fine_Road_D.dds"));
		meshComponent->getStaticMesh()->getMaterial(0).setNormalTexture(da::graphics::CTexture2DFactory::Create("assets/textures/surface/road/Tex_Fine_Road_N.dds"));
		meshComponent->getStaticMesh()->getMaterial(0).doubleSided = false;
		meshComponent->getStaticMesh()->getMaterial(0).uvScale = { 75.f, 75.f };
		meshComponent->getStaticMesh()->getMaterial(0).metallicFactor = 0.0;
		meshComponent->getStaticMesh()->getMaterial(0).roughnessFactor = 1.0;
		meshComponent->getStaticMesh()->castShadows(true);
		testBed->addComponent<da::core::CRigidBodyComponent>(
			da::physics::IPhysicsRigidBody::create(
				da::physics::CPhysicsShapeCube::create({ 100.f,100.f,0.001f })
				, da::physics::CPhysicsDefaultMotionState::create(testBed->getTransform().matrix())
				, 0.f
				, { 0.f,0.f,0.f }));
		testBed->setTag(HASHSTR("TestBed"));
		testBed->getTransform().setScale({ 1.f, 1.f, 1.f });
	}

	// Ramp1
	{
		da::core::CEntity* ramp = da::core::CSceneManager::getScene()->createEntity();
		ramp->getTransform().setPosition({ 0,30,-3 });
		ramp->getTransform().setRotation({ 15,0,0 });
		ramp->getTransform().setScale({ 5,10,5.f });
		ramp->setTag(HASHSTR("ramp1"));

		da::core::FComponentRef<da::core::CSmeshComponent> meshComponent = ramp->addComponent<da::core::CSmeshComponent>("assets/cube.fbx");
		//meshComponent->getStaticMesh()->getMaterial(0).setBaseColorTexture(da::graphics::CTexture2DFactory::Create("assets/textures/tex_debug_grid_01.png"));

		ramp->addComponent<da::core::CRigidBodyComponent>(
			da::physics::IPhysicsRigidBody::create(da::physics::CPhysicsShapeCube::create({ 5.f,10.f,5.f })
				, da::physics::CPhysicsDefaultMotionState::create(ramp->getTransform().matrix())
				, 0.f
				, { 0.f,0.f,0.f }));
	}

	// Transform obj
	{
		da::core::CEntity* transformObj = da::core::CSceneManager::getScene()->createEntity();
		transformObj->getTransform().setPosition({ 4,0,0 });
		transformObj->getTransform().setRotation({ 0,0,0 });
		transformObj->setTag(HASHSTR("transform"));

		transformObj->addComponent<da::core::CSmeshComponent>("assets/transform.fbx");
	}


	m_scrlevel.classInitialize();

	// create vehicle
	createVehicle();

	m_levelGen.generate();
}

void CTestBed03Level::update(float dt)
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
		destroyVehicle();
		createVehicle();
	}

	m_character->update(dt);
	m_vehicle->update(dt);

	m_scrlevel.classUpdate(dt);

	const std::vector<FPath> path = m_levelGen.getPath();
#ifdef DA_REVIEW
	for (const FPath& p : path) {
		da::graphics::CDebugRender::drawLine({ p.Start.X, p.Start.Y, 1.f }, { p.End.X, p.End.Y, 1.f }, .1f, { 1.f, 0.f, 1.f, 1.f }, true, true);
	}
#endif
}

void CTestBed03Level::shutdown()
{
	m_scrlevel.classShutdown();
	m_scrlevel.cleanup();

	m_character->shutdown();
	delete m_character;

	da::core::CSceneManager::setScene(nullptr);
}

void CTestBed03Level::createVehicle()
{
	// Vehicle
	m_vehicle = new CVehicle(m_graphicsModule);
	m_vehicle->initialize(&m_window, CVehicleManager::getVehicleTypes().begin()->second);
}

void CTestBed03Level::destroyVehicle()
{
	m_vehicle->shutdown();
	delete m_vehicle;
}
