#include "dapch.h"

#include "physics_vehicle.h"

#include "bullet3/bullet3_vehicle.h"

namespace da::physics
{

	da::physics::IVehicle* VehicleFactory::create(FVehicleTuning& tuning, IPhysicsRigidBody* rigidBody)
	{
		return new CBullet3Vehicle(tuning, rigidBody);
	}

}