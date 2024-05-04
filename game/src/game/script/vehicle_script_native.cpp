#include "vehicle_script_native.h"
#include <DAEngine/physics/physics_vehicle.h>

static void vehicleTestNative(sol::this_state state, sol::object _, sol::object vData) {
	da::physics::FVehicleData vehicleData = GET_SCRIPT_TYPE(da::physics::FVehicleData, vData);

	LOG_DEBUG(da::ELogChannel::Application, "Vehicle");
}


void CVehicleScriptNative::registerTypes()
{
	REGISTER_SCRIPT_TYPE(da::physics::FVehicleTuning,
		SCRIPT_TYPE(da::physics::FVehicleTuning, SuspensionStiffness, da::script::EScriptDataType::Float),
		SCRIPT_TYPE(da::physics::FVehicleTuning, SuspensionCompression, da::script::EScriptDataType::Float),
		SCRIPT_TYPE(da::physics::FVehicleTuning, SuspensionDamping, da::script::EScriptDataType::Float),
		SCRIPT_TYPE(da::physics::FVehicleTuning, MaxSuspensionTravelCm, da::script::EScriptDataType::Float),
		SCRIPT_TYPE(da::physics::FVehicleTuning, FrictionSlip, da::script::EScriptDataType::Float),
		SCRIPT_TYPE(da::physics::FVehicleTuning, MaxSuspensionForce, da::script::EScriptDataType::Float));

	REGISTER_SCRIPT_TYPE(da::physics::FWheelData,
		SCRIPT_TYPE_OBJECT(da::physics::FWheelData, WheelConnectionPoint, glm::vec3),
		SCRIPT_TYPE_OBJECT(da::physics::FWheelData, WheelDirection, glm::vec3),
		SCRIPT_TYPE_OBJECT(da::physics::FWheelData, WheelAxle, glm::vec3),
		SCRIPT_TYPE(da::physics::FWheelData, SuspensionRestLength, da::script::EScriptDataType::Float),
		SCRIPT_TYPE(da::physics::FWheelData, WheelRadius, da::script::EScriptDataType::Float),
		SCRIPT_TYPE(da::physics::FWheelData, FrontWheel, da::script::EScriptDataType::Bool));

	REGISTER_SCRIPT_TYPE(da::physics::FVehicleData,
		SCRIPT_TYPE_OBJECT(da::physics::FVehicleData, Tuning, da::physics::FVehicleTuning),
		SCRIPT_ARR_TYPE_OBJECT(da::physics::FVehicleData, Wheels, da::physics::FWheelData));
}

void CVehicleScriptNative::registerNatives(sol::state_view* view)
{
	registerTypes();
	view->set_function("vehicle_test_native", vehicleTestNative);
}


