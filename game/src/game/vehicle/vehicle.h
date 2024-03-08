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
	da::core::CEntity* m_entity;
	float m_acceleration = 0.f;
	float m_steering = 0.f;
	da::physics::IVehicle* m_vehicle;
	bool m_controlCamera = true;

};