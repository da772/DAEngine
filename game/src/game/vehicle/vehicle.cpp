#include "vehicle.h"
#include <DAEngine/physics.h>
#include <DAEngine/components.h>
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
#include <glm/gtx/vector_angle.hpp> 

#ifdef DA_REVIEW
#include <imgui.h>
#endif

CVehicle::CVehicle(uint32_t id /*= 0*/) : m_id(id)
{

}

void CVehicle::initialize(da::modules::CWindowModule* window, const glm::vec3& pos, bool proxy) {
	m_window = window;
	m_proxy = proxy;

	if (da::core::CScene* scene = da::core::CSceneManager::getScene()) {
		m_entity = scene->createEntity(da::core::CGuid::Generate(m_id));

		da::core::FComponentRef<da::core::CSmeshComponent> cc = m_entity->addComponent<da::core::CSmeshComponent>("assets/prop/veh/prop_veh_sedan/prop_veh_sedan_01a.FBX", false);
		cc->getStaticMesh()->getMaterial(0).metallicFactor = .15f;
		cc->getStaticMesh()->getMaterial(0).roughnessFactor = 1.f;
		cc->getStaticMesh()->getMaterial(0).emissiveFactor = { 1.f, 1.f,1.f };
		cc->getStaticMesh()->getMaterial(0).setBaseColorTexture(da::graphics::CTexture2DFactory::Create("assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Body_DIF.dds"));
		cc->getStaticMesh()->getMaterial(0).setMetallicRoughnessTexture(da::graphics::CTexture2DFactory::Create("assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Body_ORM.dds"));
		cc->getStaticMesh()->getMaterial(0).setNormalTexture(da::graphics::CTexture2DFactory::Create("assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Body_NRM.dds"));
		cc->getStaticMesh()->getMaterial(0).setEmissiveTexture(da::graphics::CTexture2DFactory::Create("assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Glass_EMS.dds"));
		if (!m_proxy) cc->getStaticMesh()->getMaterial(0).baseColorFactor = { .7f, .9f, .9f, 1.f };
		m_entity->addComponent<da::core::CNetworkedTransformComponent>(!m_proxy);

		cc->getStaticMesh()->getMaterial(1).baseColorFactor = { .7f, .9f, .9f, 0.f };
		cc->getStaticMesh()->getMaterial(1).metallicFactor = 0.f;
		cc->getStaticMesh()->getMaterial(1).roughnessFactor = 1.f;
		cc->getStaticMesh()->getMaterial(1).setBaseColorTexture(da::graphics::CTexture2DFactory::Create("assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Glass_DIF.dds"));
		cc->getStaticMesh()->getMaterial(1).setMetallicRoughnessTexture(da::graphics::CTexture2DFactory::Create("assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Chassis_ORM.dds"));
		cc->getStaticMesh()->getMaterial(1).setNormalTexture(da::graphics::CTexture2DFactory::Create("assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Glass_NRM.dds"));

		cc->getStaticMesh()->getMaterial(2).metallicFactor = .15f;
		cc->getStaticMesh()->getMaterial(2).roughnessFactor = 1.f;
		cc->getStaticMesh()->getMaterial(2).setBaseColorTexture(da::graphics::CTexture2DFactory::Create("assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Chassis_DIF.dds"));
		cc->getStaticMesh()->getMaterial(2).setMetallicRoughnessTexture(da::graphics::CTexture2DFactory::Create("assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Glass_ORM.dds"));
		cc->getStaticMesh()->getMaterial(2).setNormalTexture(da::graphics::CTexture2DFactory::Create("assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Chassis_NRM.dds"));
		

		cc->getStaticMesh()->getMaterial(3).metallicFactor = .15f;
		cc->getStaticMesh()->getMaterial(3).roughnessFactor = 1.f;
		cc->getStaticMesh()->getMaterial(3).setBaseColorTexture(da::graphics::CTexture2DFactory::Create("assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Interior_DIF.dds"));
		cc->getStaticMesh()->getMaterial(3).setMetallicRoughnessTexture(da::graphics::CTexture2DFactory::Create("assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Interior_ORM.dds"));
		cc->getStaticMesh()->getMaterial(3).setNormalTexture(da::graphics::CTexture2DFactory::Create("assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Interior_NRM.dds"));
		m_entity->getTransform().setPosition(pos);
		m_entity->setTag("Vehicle");

		da::core::FComponentRef<da::core::CPointLightComponent> lights = m_entity->addComponent<da::core::CPointLightComponent>();
		setHeadLights(true);
		setBrakeLights(true);

		if (m_proxy)
		{
			return;
		}

		da::graphics::CStaticMesh* collisionMesh = new da::graphics::CStaticMesh("assets/prop/veh/prop_collision_veh_sports_01a.fbx", false);
		da::core::FComponentRef<da::core::CRigidBodyComponent> rb = m_entity->addComponent<da::core::CRigidBodyComponent>(
			//da::physics::IPhysicsRigidBody::create(da::physics::IPhysicsShape::createMeshConvexHull(collisionMesh)
			da::physics::IPhysicsRigidBody::create(da::physics::CPhysicsShapeCube::create({1.f, 2.525f, .35f})
				, da::physics::CPhysicsEntityMotionState::create(m_entity)
				, 1200.f
				, { 0.f,0.f,0.f }));

		delete collisionMesh;

		da::physics::FVehicleTuning tuning;

		m_vehicle = da::physics::VehicleFactory::create(tuning, rb->getPhysicsBody());

		for (size_t i = 0; i < 4; i++) {
			da::core::CEntity* wheel = scene->createEntity();
			da::core::FComponentRef<da::core::CSmeshComponent> mesh = wheel->addComponent<da::core::CSmeshComponent>("assets/prop/veh/prop_veh_sedan/prop_veh_sedan_wheel_01a.FBX", false);
			mesh->getStaticMesh()->getMaterial(0).metallicFactor = 1.f;
			mesh->getStaticMesh()->getMaterial(0).roughnessFactor = 1.f;
			mesh->getStaticMesh()->getMaterial(0).setBaseColorTexture(da::graphics::CTexture2DFactory::Create("assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Chassis_DIF.dds"));
			mesh->getStaticMesh()->getMaterial(0).setMetallicRoughnessTexture(da::graphics::CTexture2DFactory::Create("assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Chassis_ORM.dds"));
			mesh->getStaticMesh()->getMaterial(0).setNormalTexture(da::graphics::CTexture2DFactory::Create("assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Chassis_NRM.dds"));
#ifdef DA_REVIEW
			wheel->setTag(CHashString((std::string("Wheel") + std::to_string(i)).c_str()));
#endif
			m_wheels.push_back(wheel);
		}
		
		m_window->getEventHandler().registerCallback(EEventType::InputKeyboard, BIND_EVENT_FN(CVehicle, onKeyboardInput));

		m_vehicle->applyBrake(10.f, 2);
		m_vehicle->applyBrake(10.f, 3);
	}
}

void CVehicle::update(float dt)
{

	da::core::FComponentRef<da::core::CPointLightComponent> lights = m_entity->getComponent<da::core::CPointLightComponent>();
	da::core::CCamera* cam = da::core::CCamera::getCamera();
	const glm::vec3 entityPos = m_entity->getTransform().position();
	const glm::mat4 entityT = glm::toMat4(m_entity->getTransform().rotation());

	if (m_headLightR && m_headLightL) {
		glm::vec3 rightPos = entityPos + glm::vec3(entityT * glm::vec4(0.948619f, 3.70286f, -0.011385f, 1.f));
		lights->updateLight(m_headLightR, rightPos);
		glm::vec3 leftPos = entityPos + glm::vec3(entityT * glm::vec4(-0.948619f, 3.70286f, -0.011385f, 1.f));
		lights->updateLight(m_headLightL, leftPos);
	}

	if (m_brakeLightR && m_brakeLightL) {
		lights->updateLight(m_brakeLightR, entityPos + glm::vec3(entityT * glm::vec4(0.895837f, -2.3808f, 0.110954f, 1.f)));
		lights->updateLight(m_brakeLightL, entityPos + glm::vec3(entityT * glm::vec4(-0.8925837f, -2.3808f, 0.110954f, 1.f)));
	}

	if (m_proxy) {
		return;
	}

	da::core::FComponentRef<da::core::CRigidBodyComponent> rb = m_entity->getComponent<da::core::CRigidBodyComponent>();

	updateWheels(dt);
	processInput(dt);

	float len = glm::abs(glm::length(rb->getPhysicsBody()->getLinearVelocity()));
	float mph = len * 2.237;

	const float fov = 73.7397953f;

	cam->fov = glm::min(glm::mix(fov, 90.f, mph / 165.f), 90.f);

	// ImGui
	{
		ImGui::SetNextWindowSize({ 80.f,15 });

		glm::vec3 pos = cam->matrix() * glm::vec4(entityPos, 1.0f);

		ImGui::PushStyleColor(ImGuiCol_FrameBg, { 0.f,0.f,0.f,0.f });
		ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.f,0.f,0.f,0.f });
		ImGui::SetNextWindowPos({m_window->getWindow()->getWindowData().Width/2.f - 40.f, m_window->getWindow()->getWindowData().Height/2.f +300.f});
		if (ImGui::Begin("###mph", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)) {
			auto windowWidth = ImGui::GetWindowSize().x;
			auto windowHeight = ImGui::GetWindowSize().y;
			auto textWidth = ImGui::CalcTextSize("00 mph").x;

			ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
			//ImGui::SetCursorPosY(ImGui::GetFontSize() * .25f);
			ImGui::LabelText("###mph", "%d mph", (int)mph);
			ImGui::SameLine();
			ImGui::LabelText("###ep", "%.2f power", m_enginePower);
			ImGui::SameLine();
			ImGui::LabelText("###st", "%.2f steer", m_steer);
		}
		ImGui::PopStyleColor(2);

		ImGui::End();
	}


	if (!m_controlCamera)
	{
		return;
	}

	glm::vec3 pos = glm::mix(cam->position(), entityPos + (m_entity->getTransform().forward() * -5.5f) + glm::vec3(0.f, 0.f, 3.5f), 15.f * dt);
	cam->setPosition(pos);
	glm::quat targetRot = m_entity->getTransform().rotation() * glm::angleAxis(-0.4490f, glm::vec3(1.f, 0.f, 0.f));
	glm::quat quatRot = glm::slerp(cam->rotation(), targetRot, 15.f * dt);
	cam->setRotation(quatRot);
}

void CVehicle::shutdown()
{
	da::core::CSceneManager::getScene()->removeEntity(m_entity);
	if (m_vehicle) delete m_vehicle;

	for (size_t i = 0; i < m_wheels.size(); i++) {
		da::core::CSceneManager::getScene()->removeEntity(m_wheels[i]);
	}

	m_wheels = {};

	m_window->getEventHandler().unregisterCallback(EEventType::InputKeyboard, BIND_EVENT_FN(CVehicle, onKeyboardInput));
}

da::core::CEntity* CVehicle::getEntity() const
{
	return m_entity;
}

void CVehicle::updateWheels(float dt)
{
	ASSERT(m_vehicle);
	for (size_t i = 0; i < m_wheels.size(); i++) {
		ASSERT(m_wheels[i]);
		
		const da::maths::CTransform& transform = m_wheels[i]->getTransform();
		const da::physics::FWheelTransformInfo& wheelTransform = m_vehicle->getWheelTransform(i);

		glm::vec3 pos = glm::mix(transform.position(), wheelTransform.Position, 15.f * dt);
		glm::quat rot = glm::slerp(transform.rotation(), wheelTransform.Rotation, 15.f * dt);
		
		glm::vec3 eulerAngle = glm::degrees(glm::eulerAngles(wheelTransform.Rotation));

		m_wheels[i]->getTransform().setPosition({ wheelTransform.Position.x, wheelTransform.Position.y, pos.z });
		m_wheels[i]->getTransform().setRotation(rot);
	}
}

void CVehicle::onKeyboardInput(const da::core::CEvent& e)
{
	const da::core::CInputKeyboardEvent& kb = *(da::core::CInputKeyboardEvent*)&e;

	int button = kb.getBtn();

	da::core::EInputType type = kb.getType();

	static float boost = 1.0;

	switch (button)
	{
	// C
	case 67:
	{
		if (type != da::core::EInputType::RELEASED) break;
		m_controlCamera = !m_controlCamera;
		break;
	}
	// L
	case 76:
	{
		if (type != da::core::EInputType::RELEASED) break;
		setBrakeLights(!m_brakeLightL);
		setHeadLights(!m_headLightL);
	}
	default:
		break;
	}


}

void CVehicle::processInput(float dt)
{
	bool w = da::core::CInput::inputPressed(87);
	bool s = da::core::CInput::inputPressed(83);
	// S,W
	glm::vec2 throttle = { -da::core::CInput::inputPressed(83), da::core::CInput::inputPressed(87) };
	// A,D
	glm::vec2 direction = { da::core::CInput::inputPressed(65), -da::core::CInput::inputPressed(68) };
	// Space
	float handBreak = da::core::CInput::inputPressed(32);

	float acceleration = throttle.x + throttle.y;
	float steering = direction.x + direction.y;
	
	float enginePower = acceleration * m_engineMaxPower;
	m_enginePower = std::clamp(m_enginePower + (enginePower - m_enginePower)*dt, m_engineMaxPower / -1.5f, m_engineMaxPower);

	float steer = steering * m_maxSteer;
	m_steer = std::clamp(m_steer + (steer-m_steer), -m_maxSteer, m_maxSteer);

	float brakes = 0.f;

	if (handBreak > 0.f) {
		brakes = m_brakeMaxPower;
	}
	else if (!w && !s) {
		brakes = m_idleBrake;
	}

	m_vehicle->applyEngineForce(m_enginePower, 0);
	m_vehicle->applyEngineForce(m_enginePower, 1);
	m_vehicle->setSteeringValue(m_steer, 0);
	m_vehicle->setSteeringValue(m_steer, 1);
	m_vehicle->applyBrake(brakes, 2);
	m_vehicle->applyBrake(brakes, 3);

}

void CVehicle::setBrakeLights(bool on)
{
	da::core::FComponentRef<da::core::CPointLightComponent> lights = m_entity->getComponent<da::core::CPointLightComponent>();
	if (on && !m_brakeLightL && !m_brakeLightR) {
		m_brakeLightL = lights->addLight({ 0.f,0.f,0.f }, { 2.f,0.f,0.f }, 2.f);
		m_brakeLightR = lights->addLight({ 0.f,0.f,0.f }, { 2.f,0.f,0.f }, 2.f);
		return;
	}
	
	lights->removeLight(m_brakeLightL);
	lights->removeLight(m_brakeLightR);
	m_brakeLightL = 0;
	m_brakeLightR = 0;
}

void CVehicle::setHeadLights(bool on)
{
	da::core::FComponentRef<da::core::CPointLightComponent> lights = m_entity->getComponent<da::core::CPointLightComponent>();
	if (on && !m_headLightL && !m_headLightR) {
		m_headLightR = lights->addLight({ 0.f,0.f,0.f }, glm::vec3(1.f, 0.86f, .5f) * 50.f, 15.f);
		m_headLightL = lights->addLight({ 0.f,0.f,0.f }, glm::vec3(1.f, 0.86f, .5f) * 50.f, 15.f);
		return;
	}

	lights->removeLight(m_headLightL);
	lights->removeLight(m_headLightR);
	m_headLightL = 0;
	m_headLightR = 0;
}
