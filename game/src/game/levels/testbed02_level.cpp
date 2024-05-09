// header
#include "testbed02_level.h"

//engine
#include <daengine/physics.h>
#include <daengine/graphics.h>
#include <daengine/threading.h>
#include <daengine/script.h>
#include <DAEngine/physics/physics_vehicle.h>
#include <imgui.h>

// game
#include "game/vehicle/vehicle.h"
#include "game/vehicle/vehicle_manager.h"


CTestBed02Level::CTestBed02Level(CHashString name, da::modules::CWindowModule& window) : ILevel(name), m_window(window), m_scrlevel("scripts/build/levels/test_bed_01.lua", "TestBed01", "main", false)
{

}

void CTestBed02Level::initialize()
{
	m_scrlevel.setup({}, {});
	da::core::CSceneManager::setScene(new da::core::CScene(da::core::CGuid("9059cedf-5978-4a51-991b-36d4804d55d7")));
	da::core::CCamera::getCamera()->setPosition({ 0,0,1 });

	m_scrlevel.classInitialize();

	// Vehicle
	m_playerId = da::core::CTime::getEpochTimeNS();

	m_vehicle = new CVehicle(m_playerId);
	m_vehicle->initialize(&m_window, CVehicleManager::getVehicleTypes().begin()->second, { -144.f, -80.f, -4.f });

	// Test Bed
	{
		da::core::CEntity* testBed = da::core::CSceneManager::getScene()->createEntity();
		testBed->addComponent<da::core::CScriptComponent>("scripts/build/camera_component.lua", "CameraComponent");
		da::core::FComponentRef<da::core::CSmeshComponent> meshComponent = testBed->addComponent<da::core::CSmeshComponent>("assets/prop/level/testbed.fbx");
		meshComponent->getStaticMesh()->getMaterial(0).doubleSided = false;
		meshComponent->getStaticMesh()->getMaterial(0).metallicFactor = 0.0;
		meshComponent->getStaticMesh()->getMaterial(0).roughnessFactor = 1.0;
		meshComponent->getStaticMesh()->getMaterial(0).baseColorFactor = { 128.f / 255.f,126.f / 255.f,120.f/255.f,1.f};
		meshComponent->getStaticMesh()->getMaterial(1).doubleSided = false;
		meshComponent->getStaticMesh()->getMaterial(1).metallicFactor = 0.0;
		meshComponent->getStaticMesh()->getMaterial(1).roughnessFactor = 1.0;
		meshComponent->getStaticMesh()->getMaterial(1).baseColorFactor = { 184.f / 255.f, 68.f / 255.f, 6.f/255.f, 1.f };

		meshComponent->getStaticMesh()->castShadows(true);
		testBed->setTag(HASHSTR("TestBed"));
	}

	// Test Collision
	{
		da::core::CEntity* testBed = da::core::CSceneManager::getScene()->createEntity();
		da::graphics::CStaticMesh* collision = new da::graphics::CStaticMesh("assets/prop/level/testbed.fbx");

		std::vector<da::physics::IPhysicsShape*> shapes;
		std::vector<glm::mat4> shapestrans;

		for (size_t i = 0; i < collision->getMeshes().size(); i++) {
			shapes.push_back(da::physics::CPhysicsShapeTriangleMesh::create(collision, i));
			shapestrans.push_back(glm::mat4(1.f));
		}

		testBed->addComponent<da::core::CRigidBodyComponent>(
			da::physics::IPhysicsRigidBody::create(
				da::physics::CPhysicsShapeCompound::create(shapes, shapestrans)
				, da::physics::CPhysicsDefaultMotionState::create(testBed->getTransform().matrix())
				, 0.f
				, { 0.f,0.f,0.f }));
		testBed->setTag(HASHSTR("TestBedCollision"));
	}

	if (m_network = da::net::CNetworkManager::getNetwork()) {
		m_network->getPacket<FPlayerJoinPacketInfo>(PLAYER_JOIN_PACKET_ID, [this](FPlayerJoinPacketInfo data) {

			const std::unordered_map<uint64_t, CVehicle*>::iterator& it = m_vehicles.find(data.PlayerId);
			if (it != m_vehicles.end()) return;

			LOG_DEBUG(da::ELogChannel::Application, "Player Joined: %ull", data.PlayerId);
			m_vehicles[data.PlayerId] = new CVehicle((uint32_t)data.PlayerId);
			m_vehicles[data.PlayerId]->initialize(&m_window, CVehicleManager::getVehicleTypes().begin()->second,{-144.f, -80.f, -4.f}, true);


			m_pushPlayerId = true;
		});

		m_network->getPacket<FPlayerLeavePacketInfo>(PLAYER_LEAVE_PACKET_ID, [this](FPlayerLeavePacketInfo data) {
			LOG_DEBUG(da::ELogChannel::Application, "Player Left: %ull", data.PlayerId);
			const std::unordered_map<uint64_t, CVehicle*>::iterator& it = m_vehicles.find(data.PlayerId);
			if (it != m_vehicles.end()) {
				it->second->shutdown();
				delete it->second;
				m_vehicles.erase(it);
			}
		});

		LOG_DEBUG(da::ELogChannel::Application, "Sending Player Joined Packet: %ull", m_playerId);
		m_network->sendPacket<FPlayerJoinPacketInfo>(FPlayerJoinPacketInfo(m_playerId));
	}


}


void CTestBed02Level::update(float dt)
{
	m_vehicle->update(dt);

	for (const auto& kv : m_vehicles) {
		kv.second->update(dt);
	}

	m_timePassed += dt;

	if (m_pushPlayerId)
	{
		m_pushPlayerId = false;
		m_network->sendPacket<FPlayerJoinPacketInfo>(FPlayerJoinPacketInfo(m_playerId));
	}

}

void CTestBed02Level::shutdown()
{
	m_vehicle->shutdown();
	delete m_vehicle;

	if (m_network) {
		m_network->clearPackets();
		m_network->sendPacket<FPlayerLeavePacketInfo>(FPlayerLeavePacketInfo(m_playerId));
	}

	for (const auto& kv : m_vehicles) {
		delete kv.second;
	}

	m_vehicles = {};

	
	m_scrlevel.classShutdown();
	da::core::CSceneManager::setScene(nullptr);
}
