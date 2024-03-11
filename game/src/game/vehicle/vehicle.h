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

class CVehicle
{
public:
	void initialize();
	void update(float dt);
	void shutdown();

private:
	void updateWheels(float dt);

private:
	da::core::CEntity* m_entity;
	std::vector<da::core::CEntity*> m_wheels;
	float m_acceleration = 0.f;
	float m_steering = 0.f;
	da::physics::IVehicle* m_vehicle;
	bool m_controlCamera = true;

};