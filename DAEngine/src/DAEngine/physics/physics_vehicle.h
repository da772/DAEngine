#pragma once
#include <graphics/graphics_material_data.h>
#include "dastd.h"
#include "daglm.h"

namespace da
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

	struct FWheelData
	{
		glm::vec3 WheelConnectionPoint;
		glm::vec3 WheelDirection;
		glm::vec3 WheelAxle;
		float SuspensionRestLength;
		float WheelRadius;
		bool FrontWheel;
	};

	struct FVehicleTuningData
	{
		FVehicleTuning Tuning;
		std::vector<FWheelData> Wheels;
	};

	struct FWheelTransformInfo
	{
		glm::vec3 Position;
		glm::quat Rotation;
		glm::mat4 Transform;
	};

	struct FVehicleData
	{
		da::FVehicleTuningData TuningData;
		std::vector<da::FMaterialData> Materials;
		std::string VehicleMesh;
	};


	class IVehicle
	{
	public:
		inline IVehicle(const FVehicleData& data, IPhysicsRigidBody* rigidBody) {};
		inline virtual ~IVehicle() {};

		virtual void setSteeringValue(float value, size_t wheelIndex) = 0;
		virtual void applyEngineForce(float value, size_t wheelIndex) = 0;
		virtual void applyBrake(float value, size_t wheelIndex) = 0;
		virtual FWheelTransformInfo getWheelTransform(size_t wheelIndex) = 0;
	};

	class VehicleFactory
	{
	public:
		static IVehicle* create(const FVehicleData& tuning, IPhysicsRigidBody* rigidBody);
	};
}