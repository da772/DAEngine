

#include "physics/physics_vehicle.h"

#include "bullet3_vehicle.h"

namespace da
{

	da::IVehicle* VehicleFactory::create(const FVehicleData& data, IPhysicsRigidBody* rigidBody)
	{
		return new CBullet3Vehicle(data, rigidBody);
	}

}