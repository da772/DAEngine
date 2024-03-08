#include "dapch.h"
#include "bullet3_vehicle.h"
#include "physics_bullet3_rigid_body.h"
#include "bullet3_physics.h"
#include <bullet3/btBulletDynamicsCommon.h>


namespace da::physics
{

	CBullet3Vehicle::CBullet3Vehicle(FVehicleTuning& tuning, IPhysicsRigidBody* rigidBody) : IVehicle(tuning, rigidBody)
	{
		CBullet3Physics* physics = dynamic_cast<CBullet3Physics*>(CPhysics::getPhysicsType());
		ASSERT(physics);


		CBullet3RigidBody* btRigidBody = dynamic_cast<CBullet3RigidBody*>(rigidBody);
		ASSERT(btRigidBody);

		btVehicleRaycaster* rayCaster = new btDefaultVehicleRaycaster(physics->getDynamicsWorld());

		m_tuning = tuning;

		btRaycastVehicle::btVehicleTuning btTuning;
		btTuning.m_frictionSlip = m_tuning.FrictionSlip;
		btTuning.m_maxSuspensionForce = m_tuning.MaxSuspensionForce;
		btTuning.m_maxSuspensionTravelCm = m_tuning.MaxSuspensionTravelCm;
		btTuning.m_suspensionDamping = m_tuning.SuspensionDamping;
		btTuning.m_suspensionStiffness = m_tuning.SuspensionStiffness;
		btTuning.m_suspensionCompression = m_tuning.SuspensionDamping;

		m_vehicle = new btRaycastVehicle(btTuning, btRigidBody->getRigidBody(), rayCaster);

		btRigidBody->getRigidBody()->setActivationState(DISABLE_DEACTIVATION);;
		physics->getDynamicsWorld()->addVehicle(m_vehicle);

		addWheels();
	}

	CBullet3Vehicle::~CBullet3Vehicle()
	{
		ASSERT(m_vehicle);
		delete m_vehicle;
		m_vehicle = nullptr;

		ASSERT(m_raycaster);
		delete m_raycaster;
		m_raycaster = nullptr;
	}

	void CBullet3Vehicle::addWheels()
	{
		btRaycastVehicle::btVehicleTuning btTuning;
		btTuning.m_frictionSlip = m_tuning.FrictionSlip;
		btTuning.m_maxSuspensionForce = m_tuning.MaxSuspensionForce;
		btTuning.m_maxSuspensionTravelCm = m_tuning.MaxSuspensionTravelCm;
		btTuning.m_suspensionDamping = m_tuning.SuspensionDamping;
		btTuning.m_suspensionStiffness = m_tuning.SuspensionStiffness;
		btTuning.m_suspensionCompression = m_tuning.SuspensionDamping;

		//The direction of the raycast, the btRaycastVehicle uses raycasts instead of simiulating the wheels with rigid bodies
		btVector3 wheelDirectionCS0(0, 0, -1);

		//The axis which the wheel rotates arround
		btVector3 wheelAxleCS(1, 0, 0);

		btScalar suspensionRestLength(0.15);
		btScalar wheelWidth(0.1);
		btScalar wheelRadius(0.25);

		btVector3 extends = { .65f,  1.25f, -.2f };

		btVector3 wheelConnectionPoint = { .65f,  1.25f, -.2f };

		// FR
		m_vehicle->addWheel(wheelConnectionPoint, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, btTuning, true);

		//FL
		m_vehicle->addWheel(wheelConnectionPoint * btVector3(-1, 1, 1), wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, btTuning, true);

		// BR
		m_vehicle->addWheel(wheelConnectionPoint * btVector3(1, -1, 1), wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, btTuning, true);

		// BL
		m_vehicle->addWheel(wheelConnectionPoint * btVector3(-1, -1, 1), wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, btTuning, true);

		for (int i = 0; i < m_vehicle->getNumWheels(); i++)
		{
			btWheelInfo& wheel = m_vehicle->getWheelInfo(i);
			wheel.m_suspensionStiffness = 50;
			wheel.m_wheelsDampingCompression = btScalar(0.3) * 2 * btSqrt(wheel.m_suspensionStiffness);//btScalar(0.8);
			wheel.m_wheelsDampingRelaxation = btScalar(0.5) * 2 * btSqrt(wheel.m_suspensionStiffness);//1;
			//Larger friction slips will result in better handling
			wheel.m_frictionSlip = btScalar(1.2);
			wheel.m_rollInfluence = 1;
		}
	}

	void CBullet3Vehicle::setSteeringValue(float value, size_t wheelIndex)
	{
		ASSERT(m_vehicle);
		ASSERT(m_vehicle->getNumWheels() > wheelIndex);

		m_vehicle->setSteeringValue(value, wheelIndex);
	}

	void CBullet3Vehicle::applyEngineForce(float value, size_t wheelIndex)
	{
		ASSERT(m_vehicle);
		ASSERT(m_vehicle->getNumWheels() > wheelIndex);

		m_vehicle->applyEngineForce(value, wheelIndex);
	}

	void CBullet3Vehicle::applyBrake(float value, size_t wheelIndex)
	{
		ASSERT(m_vehicle);
		ASSERT(m_vehicle->getNumWheels() > wheelIndex);

		m_vehicle->setBrake(value, wheelIndex);
	}

}