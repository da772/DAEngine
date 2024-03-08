#pragma once
#include "DAEngine/physics/physics_vehicle.h"

class IPhysicsRigidBody;
struct btRaycastVehicle;
class btVehicleRaycaster;

namespace da::physics
{
	class CBullet3Vehicle : public IVehicle {

	public:
		CBullet3Vehicle(FVehicleTuning& tuning, IPhysicsRigidBody* rigidBody);
		~CBullet3Vehicle();

		void setSteeringValue(float value, size_t wheelIndex);
		void applyEngineForce(float value, size_t wheelIndex);
		void applyBrake(float value, size_t wheelIndex);

	private:
		void addWheels();

	private:
		btRaycastVehicle* m_vehicle;
		FVehicleTuning m_tuning;
		btVehicleRaycaster* m_raycaster;

	};
}