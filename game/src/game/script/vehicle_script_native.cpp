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
	da::FVehicleData vehicleData = GET_SCRIPT_TYPE(da::FVehicleData, vData);

	CVehicleManager::addVehicleType(id, vehicleData);
}

static void unRegisterVehicle(sol::this_state state, sol::object _, std::string strId, bool cleanup) {
	CHashString id = CHashString(strId.c_str(), strId.size());

	CVehicleManager::removeVehicleType(id, cleanup);
}

void CVehicleScriptNative::registerTypes()
{
	REGISTER_SCRIPT_TYPE(da::FVehicleTuning,
		SCRIPT_TYPE(da::FVehicleTuning, SuspensionStiffness, da::EScriptDataType::Float),
		SCRIPT_TYPE(da::FVehicleTuning, SuspensionCompression, da::EScriptDataType::Float),
		SCRIPT_TYPE(da::FVehicleTuning, SuspensionDamping, da::EScriptDataType::Float),
		SCRIPT_TYPE(da::FVehicleTuning, MaxSuspensionTravelCm, da::EScriptDataType::Float),
		SCRIPT_TYPE(da::FVehicleTuning, FrictionSlip, da::EScriptDataType::Float),
		SCRIPT_TYPE(da::FVehicleTuning, MaxSuspensionForce, da::EScriptDataType::Float));

	REGISTER_SCRIPT_TYPE(da::FWheelData,
		SCRIPT_TYPE_OBJECT(da::FWheelData, WheelConnectionPoint, glm::vec3),
		SCRIPT_TYPE_OBJECT(da::FWheelData, WheelDirection, glm::vec3),
		SCRIPT_TYPE_OBJECT(da::FWheelData, WheelAxle, glm::vec3),
		SCRIPT_TYPE(da::FWheelData, SuspensionRestLength, da::EScriptDataType::Float),
		SCRIPT_TYPE(da::FWheelData, WheelRadius, da::EScriptDataType::Float),
		SCRIPT_TYPE(da::FWheelData, FrontWheel, da::EScriptDataType::Bool));

	REGISTER_SCRIPT_TYPE(da::FVehicleTuningData,
		SCRIPT_TYPE_OBJECT(da::FVehicleTuningData, Tuning, da::FVehicleTuning),
		SCRIPT_ARR_TYPE_OBJECT(da::FVehicleTuningData, Wheels, da::FWheelData));

	REGISTER_SCRIPT_TYPE(da::FVehicleData,
		SCRIPT_TYPE_OBJECT(da::FVehicleData, TuningData, da::FVehicleTuningData),
		SCRIPT_ARR_TYPE_OBJECT(da::FVehicleData, Materials, da::FMaterialData),
		SCRIPT_TYPE(da::FVehicleData, VehicleMesh, da::EScriptDataType::String)
	);
}

void CVehicleScriptNative::registerNatives(sol::state_view* view)
{
	registerTypes();
	view->set_function("register_vehicle", registerVehicle);
	view->set_function("unregister_vehicle", unRegisterVehicle);
}


