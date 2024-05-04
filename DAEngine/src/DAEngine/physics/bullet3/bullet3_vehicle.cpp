#include "dapch.h"
#include "bullet3_vehicle.h"
#include "physics_bullet3_rigid_body.h"
#include "bullet3_physics.h"
#include <bullet3/btBulletDynamicsCommon.h>


namespace da::physics
{

	CBullet3Vehicle::CBullet3Vehicle(const FVehicleData& tuning, IPhysicsRigidBody* rigidBody) : IVehicle(tuning, rigidBody)
	{
		CBullet3Physics* physics = dynamic_cast<CBullet3Physics*>(CPhysics::getPhysicsType());
		ASSERT(physics);


		CBullet3RigidBody* btRigidBody = dynamic_cast<CBullet3RigidBody*>(rigidBody);
		ASSERT(btRigidBody);

		btVehicleRaycaster* rayCaster = new btDefaultVehicleRaycaster(physics->getDynamicsWorld());

		m_tuning = tuning;

		btRaycastVehicle::btVehicleTuning btTuning;
		btTuning.m_frictionSlip = m_tuning.Tuning.FrictionSlip;
		btTuning.m_maxSuspensionForce = m_tuning.Tuning.MaxSuspensionForce;
		btTuning.m_maxSuspensionTravelCm = m_tuning.Tuning.MaxSuspensionTravelCm;
		btTuning.m_suspensionDamping = m_tuning.Tuning.SuspensionDamping;
		btTuning.m_suspensionStiffness = m_tuning.Tuning.SuspensionStiffness;
		btTuning.m_suspensionCompression = m_tuning.Tuning.SuspensionDamping;

		m_vehicle = new btRaycastVehicle(btTuning, btRigidBody->getRigidBody(), rayCaster);

		btRigidBody->getRigidBody()->setActivationState(DISABLE_DEACTIVATION);;
		physics->getDynamicsWorld()->addVehicle(m_vehicle);

		addWheels();
	}

	CBullet3Vehicle::~CBullet3Vehicle()
	{
		CBullet3Physics* physics = dynamic_cast<CBullet3Physics*>(CPhysics::getPhysicsType());
		ASSERT(physics);
		ASSERT(m_vehicle);
		physics->getDynamicsWorld()->removeVehicle(m_vehicle);
		delete m_vehicle;
		m_vehicle = nullptr;

		m_raycaster = nullptr;
	}

	void CBullet3Vehicle::addWheels()
	{
		btRaycastVehicle::btVehicleTuning btTuning;
		btTuning.m_frictionSlip = m_tuning.Tuning.FrictionSlip;
		btTuning.m_maxSuspensionForce = m_tuning.Tuning.MaxSuspensionForce;
		btTuning.m_maxSuspensionTravelCm = m_tuning.Tuning.MaxSuspensionTravelCm;
		btTuning.m_suspensionDamping = m_tuning.Tuning.SuspensionDamping;
		btTuning.m_suspensionStiffness = m_tuning.Tuning.SuspensionStiffness;
		btTuning.m_suspensionCompression = m_tuning.Tuning.SuspensionDamping;

		for (const FWheelData& wheel : m_tuning.Wheels) {
			btWheelInfo& wheelInfo = m_vehicle->addWheel(
				{ wheel.WheelConnectionPoint.x, wheel.WheelConnectionPoint.y, wheel.WheelConnectionPoint.z }
				, { wheel.WheelDirection.x, wheel.WheelDirection.y, wheel.WheelDirection.z }
				, { wheel.WheelAxle.x, wheel.WheelAxle.y, wheel.WheelAxle.z }
				, wheel.SuspensionRestLength
				, wheel.WheelRadius
				, btTuning
				, wheel.FrontWheel);

			wheelInfo.m_suspensionStiffness = 50;
			wheelInfo.m_wheelsDampingCompression = btScalar(0.3) * 2 * btSqrt(wheelInfo.m_suspensionStiffness);//btScalar(0.8);
			wheelInfo.m_wheelsDampingRelaxation = btScalar(0.5) * 2 * btSqrt(wheelInfo.m_suspensionStiffness);//1;
			//Larger friction slips will result in better handling
			wheelInfo.m_frictionSlip = btScalar(1.2);
			wheelInfo.m_rollInfluence = .5f;
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

	FWheelTransformInfo CBullet3Vehicle::getWheelTransform(size_t wheelIndex)
	{
		ASSERT(m_vehicle);
		ASSERT(m_vehicle->getNumWheels() > wheelIndex);

		const btTransform btTrans = m_vehicle->getWheelTransformWS(wheelIndex);

		glm::mat4 trans;
		btTrans.getOpenGLMatrix(glm::value_ptr(trans));

		glm::vec3 scale, translation, skew;
		glm::vec4 perspective;
		glm::quat orientation;

		glm::decompose(trans, scale, orientation, translation, skew, perspective);

		return {translation, orientation, trans};
	}

}