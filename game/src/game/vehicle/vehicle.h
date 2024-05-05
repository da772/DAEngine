#pragma once

#include <DAEngine/core.h>
#include <daengine/physics.h>
#include "daengine/graphics.h"

namespace da::core
{
	class CEntity;
}

namespace da::physics
{
	struct FHitData;
	class IVehicle;
}

namespace da::modules
{
	class CWindowModule;
}

class CVehicle
{
public:
	CVehicle(uint32_t id = 0);
	void initialize(da::modules::CWindowModule* window, const da::physics::FVehicleData& vehicleData, const glm::vec3& pos = {0.f,0.f,1.f}, bool proxy = false);
	void update(float dt);
	void shutdown();
	da::core::CEntity* getEntity() const;

private:
	void updateWheels(float dt);
	void onKeyboardInput(const da::core::CEvent& e);
	void setBrakeLights(bool on);
	void setHeadLights(bool on);
	void updateLights();
	void processInput(float dt);

private:
	da::core::CEntity* m_entity;
	std::vector<da::core::CEntity*> m_wheels;
	float m_acceleration = 0.f;
	float m_steering = 0.f;
	da::physics::IVehicle* m_vehicle = nullptr;
	bool m_controlCamera = true;
	da::modules::CWindowModule* m_window;

	bool m_proxy;

	float m_engineMaxPower = 3000.f;
	float m_brakeMaxPower = 300.f;
	float m_maxSteer = 0.45f;
	float m_idleBrake = 10.f;

	float m_enginePower = 0.f;
	float m_steer = 0.f;

	uint32_t m_brakeLightL = 0, m_brakeLightR = 0;
	uint32_t m_headLightL = 0, m_headLightR = 0;

	uint32_t m_id;

};