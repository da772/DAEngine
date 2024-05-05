//header
#include "vehicle_script_native.h"

// engine
#include <DAEngine/physics.h>
#include <DAEngine/graphics.h>

// game
#include "game/vehicle/vehicle_manager.h"
#include "game/vehicle/vehicle.h"



static void registerVehicle(sol::this_state state, sol::object _, std::string strId, sol::object vData) {
	CHashString id = CHashString(strId.c_str(), strId.size());
	da::physics::FVehicleData vehicleData = GET_SCRIPT_TYPE(da::physics::FVehicleData, vData);

	CVehicleManager::addVehicleType(id, vehicleData);
}

static void unRegisterVehicle(sol::this_state state, sol::object _, std::string strId) {
	CHashString id = CHashString(strId.c_str(), strId.size());

	CVehicleManager::removeVehicleType(id);
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

	REGISTER_SCRIPT_TYPE(da::physics::FVehicleTuningData,
		SCRIPT_TYPE_OBJECT(da::physics::FVehicleTuningData, Tuning, da::physics::FVehicleTuning),
		SCRIPT_ARR_TYPE_OBJECT(da::physics::FVehicleTuningData, Wheels, da::physics::FWheelData));

	REGISTER_SCRIPT_TYPE(da::physics::FVehicleData,
		SCRIPT_TYPE_OBJECT(da::physics::FVehicleData, TuningData, da::physics::FVehicleTuningData),
		SCRIPT_ARR_TYPE_OBJECT(da::physics::FVehicleData, Materials, da::graphics::FMaterialData),
		SCRIPT_TYPE(da::physics::FVehicleData, VehicleMesh, da::script::EScriptDataType::String)
	);
}

void CVehicleScriptNative::registerNatives(sol::state_view* view)
{
	registerTypes();
	view->set_function("register_vehicle", registerVehicle);
	view->set_function("unregister_vehicle", unRegisterVehicle);
}


