#pragma once


namespace da::physics
{
	class CVehicleTuning;
	class IPhysicsRigidBody;
	
	struct FVehicleTuning
	{
		float SuspensionStiffness = 5.88f;
		float SuspensionCompression = 0.83f;
		float SuspensionDamping = 0.88f;
		float MaxSuspensionTravelCm = 500.f;
		float FrictionSlip = 10.5f;
		float MaxSuspensionForce = 6000.f;
	};

	class IVehicle
	{
	public:
		inline IVehicle(FVehicleTuning& tuning, IPhysicsRigidBody* rigidBody) {};
		inline virtual ~IVehicle() {};

		virtual void setSteeringValue(float value, size_t wheelIndex) = 0;
		virtual void applyEngineForce(float value, size_t wheelIndex) = 0;
		virtual void applyBrake(float value, size_t wheelIndex) = 0;
	};

	class VehicleFactory
	{
	public:
		static IVehicle* create(FVehicleTuning& tuning, IPhysicsRigidBody* rigidBody);
	};
}