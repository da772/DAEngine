#include "vehicle.h"
#include <DAEngine/physics/physics_shape.h>
#include <DAEngine/physics/physics_motion_state.h>
#include <DAEngine/core/graphics/factory/factory_graphics_texture2d.h>

#include <DAEngine/physics/physics.h>
#include <DAEngine/physics/physics_shape.h>
#include <DAEngine/physics/physics_rigid_body.h>
#include <DAEngine/core/ecs/rigid_body_component.h>
#include <DAEngine/core/graphics/graphics_debug_render.h>
#include <DAEngine/core/graphics/camera.h>
#include <DAEngine/physics/physics_vehicle.h>
#include <DAEngine/core/input/input.h>
#include <DAEngine/core/graphics/graphics_smesh.h>

void CVehicle::initialize()
{
	if (da::core::CScene* scene = da::core::CSceneManager::getScene()) {
		m_entity = scene->createEntity();

		da::core::FComponentRef<da::core::CSmeshComponent> cc = m_entity->addComponent<da::core::CSmeshComponent>("assets/car/car1.fbx", false);
		cc->getStaticMesh()->getMaterial(0).baseColorFactor = { 1.f,0.f,0.f,1.f };
		cc->getStaticMesh()->getMaterial(0).metallicFactor = .5f;
		cc->getStaticMesh()->getMaterial(0).roughnessFactor = .5f;
		//m_entity->getTransform().setScale({ 2.f, 1.f, .55f });
		m_entity->getTransform().setPosition({ 0.f,0.f,1.f });
		m_entity->setTag("Vehicle");

		da::core::FComponentRef<da::core::CRigidBodyComponent> rb = m_entity->addComponent<da::core::CRigidBodyComponent>(
			da::physics::IPhysicsRigidBody::create(da::physics::CPhysicsShapeConvexHull::create(cc->getStaticMesh())
				, da::physics::CPhysicsEntityMotionState::create(m_entity)
				, 1200.f
				, { 0.f,0.f,0.f }));

		da::physics::FVehicleTuning tuning;

		m_vehicle = da::physics::VehicleFactory::create(tuning, rb->getPhysicsBody());
	}
}

void CVehicle::update(float dt)
{
	da::core::FComponentRef<da::core::CRigidBodyComponent> rb = m_entity->getComponent<da::core::CRigidBodyComponent>();
	const glm::vec3 entityPos = m_entity->getTransform().position();

	// up
	if (da::core::CInput::inputPressed(265)) {
		m_vehicle->applyEngineForce(1000.f, 2);
		m_vehicle->applyEngineForce(1000.f, 3);
	}
	// down 
	else if (da::core::CInput::inputPressed(264)) {
		m_vehicle->applyEngineForce(-500.f, 2);
		m_vehicle->applyEngineForce(-500.f, 3);
	}
	else if (da::core::CInput::inputPressed(32))
	{
		m_vehicle->applyBrake(500.f, 2);
		m_vehicle->applyBrake(500.f, 3);
	}
	else {
		m_vehicle->applyEngineForce(0.f, 2);
		m_vehicle->applyEngineForce(0.f, 3);
		m_vehicle->applyBrake(10.f, 2);
		m_vehicle->applyBrake(10.f, 3);
	}

	
	// Left
	if (da::core::CInput::inputPressed(263)) {
		m_vehicle->setSteeringValue(.45f, 0);
		m_vehicle->setSteeringValue(.45f, 1);
	}
	// right
	else if (da::core::CInput::inputPressed(262)) {
		m_vehicle->setSteeringValue(-.45f, 0);
		m_vehicle->setSteeringValue(-.45f, 1);
	}
	else {
		m_vehicle->setSteeringValue(0, 0);
		m_vehicle->setSteeringValue(0, 1);
	}

	// C
	if (da::core::CInput::inputPressed(67))
	{
		m_controlCamera = !m_controlCamera;
	}

	if (!m_controlCamera)
	{
		return;
	}


	da::core::CCamera* cam = da::core::CCamera::getCamera();
	glm::vec3 pos = glm::mix(cam->position(), entityPos + (m_entity->getTransform().forward() * -4.5f) + glm::vec3(0.f, 0.f, 3.5f), 15.f * dt);
	cam->setPosition(pos);
	glm::vec3 rot = glm::eulerAngles(m_entity->getTransform().rotation());
	glm::quat quatRot = glm::slerp(cam->rotation(), glm::quat(glm::radians(glm::vec3(-20.f, 0.f, glm::degrees(rot.z)))), 15.f * dt);
	cam->setRotation(quatRot);
}

void CVehicle::shutdown()
{
	da::core::CSceneManager::getScene()->removeEntity(m_entity);
	delete m_vehicle;
}
