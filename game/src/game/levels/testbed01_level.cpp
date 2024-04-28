#include "testbed01_level.h"

//engine
#include <daengine/physics.h>
#include <daengine/graphics.h>

// game
#include "game/character/character.h"
#include "game/vehicle/vehicle.h"


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

CTestBed01Level::CTestBed01Level(CHashString name, da::modules::CWindowModule& window) : ILevel(name), m_window(window)
{

}

void CTestBed01Level::initialize()
{
	da::core::CSceneManager::setScene(new da::core::CScene(da::core::CGuid("2f94546a-a92a-4046-946a-f28c2364110b")));
	da::core::CCamera::getCamera()->setPosition({ 0,0,1 });

	// Character
	m_character = new CCharacter();
	m_character->initialize();

	// Vehicle
	m_vehicle = new CVehicle();
	m_vehicle->initialize(&m_window);

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
		meshComponent->getStaticMesh()->getMaterial(0).uvScale = {75.f, 75.f};
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
		meshComponent->getStaticMesh()->getMaterial(0).setBaseColorTexture(da::graphics::CTexture2DFactory::Create("assets/textures/tex_debug_grid_01.png"));

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

	{
		da::core::CEntity* arena = da::core::CSceneManager::getScene()->createEntity();
		arena->getTransform().setPosition({ 0,0,0.f });
		arena->getTransform().setRotation({ 0,0,0 });
		arena->setTag(HASHSTR("arena"));

		da::core::FComponentRef<da::core::CSmeshComponent> meshComponent = arena->addComponent<da::core::CSmeshComponent>("assets/prop/structure/prop_gladiator_arena.fbx");
		meshComponent->getStaticMesh()->castShadows(false);

		da::graphics::CStaticMesh* collision = new da::graphics::CStaticMesh("assets/prop/structure/prop_collision_gladiator_arena.fbx");
		arena->addComponent<da::core::CRigidBodyComponent>(da::physics::IPhysicsRigidBody::create(
			da::physics::CPhysicsShapeTriangleMesh::create(collision)
			, da::physics::CPhysicsDefaultMotionState::create(arena->getTransform().matrix())
			, 0.f
			, { 0.f,0.f,0.f }));

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
}

void CTestBed01Level::update(float dt)
{
	m_character->update(dt);
	m_vehicle->update(dt);
}

void CTestBed01Level::shutdown()
{
	m_character->shutdown();
	delete m_character;

	m_vehicle->shutdown();
	delete m_vehicle;

	da::core::CSceneManager::setScene(nullptr);
}
