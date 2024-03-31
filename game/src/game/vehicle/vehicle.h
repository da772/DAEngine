#pragma once

#include <DAEngine.h>


namespace da::core
{
	class CEntity;
}

namespace da::physics
{
	class FHitData;
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

private:
	da::core::CEntity* m_entity;
	std::vector<da::core::CEntity*> m_wheels;
	float m_acceleration = 0.f;
	float m_steering = 0.f;
	da::physics::IVehicle* m_vehicle;
	bool m_controlCamera = true;
	da::modules::CWindowModule* m_window;

	float m_enginePower = 0.f;
	float m_brakePower = 0.f;
	float m_steerAmt = 0.f;

	uint32_t m_brakeLightL = 0, m_brakeLightR = 0;
	uint32_t m_headLightL = 0, m_headLightR = 0;

};