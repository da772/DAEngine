#pragma once
#include "physics/physics_vehicle.h"

class IPhysicsRigidBody;
struct btRaycastVehicle;
class btVehicleRaycaster;

namespace da::physics
{
	class CBullet3Vehicle : public IVehicle {

	public:
		CBullet3Vehicle(const FVehicleData& tuning, IPhysicsRigidBody* rigidBody);
		~CBullet3Vehicle();

		virtual void setSteeringValue(float value, size_t wheelIndex) override;
		virtual void applyEngineForce(float value, size_t wheelIndex) override;
		virtual void applyBrake(float value, size_t wheelIndex) override;

		virtual FWheelTransformInfo getWheelTransform(size_t wheelIndex) override;

	private:
		void addWheels();

	private:
		btRaycastVehicle* m_vehicle;
		FVehicleData m_tuning;
		btVehicleRaycaster* m_raycaster;

	};
}