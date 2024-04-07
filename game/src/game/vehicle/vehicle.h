#pragma once

#include <DAEngine/core.h>


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
	void initialize(da::modules::CWindowModule* window);
	void update(float dt);
	void shutdown();

private:
	void updateWheels(float dt);
	void onKeyboardInput(const da::core::CEvent& e);
	void setBrakeLights(bool on);
	void setHeadLights(bool on);
	void processInput(float dt);

private:
	da::core::CEntity* m_entity;
	std::vector<da::core::CEntity*> m_wheels;
	float m_acceleration = 0.f;
	float m_steering = 0.f;
	da::physics::IVehicle* m_vehicle;
	bool m_controlCamera = true;
	da::modules::CWindowModule* m_window;

	float m_engineMaxPower = 3000.f;
	float m_brakeMaxPower = 300.f;
	float m_maxSteer = 0.45f;
	float m_idleBrake = 10.f;

	float m_enginePower = 0.f;
	float m_steer = 0.f;

	uint32_t m_brakeLightL = 0, m_brakeLightR = 0;
	uint32_t m_headLightL = 0, m_headLightR = 0;

};