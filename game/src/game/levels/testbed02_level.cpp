#include "testbed02_level.h"

//engine
#include <daengine/physics.h>
#include <daengine/graphics.h>

// game
#include "game/vehicle/vehicle.h"

CTestBed02Level::CTestBed02Level(CHashString name, da::modules::CWindowModule& window) : ILevel(name), m_window(window)
{

}

void CTestBed02Level::initialize()
{
	da::core::CSceneManager::setScene(new da::core::CScene(da::core::CGuid("9059cedf-5978-4a51-991b-36d4804d55d7")));
	da::core::CCamera::getCamera()->setPosition({ 0,0,1 });

	// Vehicle
	m_vehicle = new CVehicle();
	m_vehicle->initialize(&m_window);

	// Test Bed
	{
		da::core::CEntity* testBed = da::core::CSceneManager::getScene()->createEntity();
		testBed->addComponent<da::core::CScriptComponent>("scripts/build/camera_component.lua", "CameraComponent");
		da::core::FComponentRef<da::core::CSmeshComponent> meshComponent = testBed->addComponent<da::core::CSmeshComponent>("assets/prop/level/testbed.fbx");
		meshComponent->getStaticMesh()->getMaterial(0).doubleSided = false;
		meshComponent->getStaticMesh()->getMaterial(0).metallicFactor = 0.0;
		meshComponent->getStaticMesh()->getMaterial(0).roughnessFactor = 1.0;
		meshComponent->getStaticMesh()->getMaterial(1).doubleSided = false;
		meshComponent->getStaticMesh()->getMaterial(1).metallicFactor = 0.0;
		meshComponent->getStaticMesh()->getMaterial(1).roughnessFactor = 1.0;
		meshComponent->getStaticMesh()->getMaterial(1).uvScale = { 11.0f, 11.f };
		meshComponent->getStaticMesh()->castShadows(false);
		testBed->addComponent<da::core::CRigidBodyComponent>(
			da::physics::IPhysicsRigidBody::create(
				da::physics::CPhysicsShapeCube::create({ 165.f,165.f,0.001f })
				, da::physics::CPhysicsDefaultMotionState::create(testBed->getTransform().matrix())
				, 0.f
				, { 0.f,0.f,0.f }));
		testBed->setTag(HASHSTR("TestBed"));
	}

	// Test Collision
	{
		da::core::CEntity* testBed = da::core::CSceneManager::getScene()->createEntity();
		da::graphics::CStaticMesh* collision = new da::graphics::CStaticMesh("assets/prop/level/testbed_collision.fbx");
		testBed->addComponent<da::core::CRigidBodyComponent>(
			da::physics::IPhysicsRigidBody::create(
				da::physics::CPhysicsShapeTriangleMesh::create(collision)
				, da::physics::CPhysicsDefaultMotionState::create(testBed->getTransform().matrix())
				, 0.f
				, { 0.f,0.f,0.f }));
		testBed->setTag(HASHSTR("TestBedCollision"));
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
}

void CTestBed02Level::update(float dt)
{
	m_vehicle->update(dt);
}

void CTestBed02Level::shutdown()
{
	m_vehicle->shutdown();
	delete m_vehicle;

	da::core::CSceneManager::setScene(nullptr);
}
