
#include "vehicle_manager.h"

void CVehicleManager::addVehicleType(CHashString id, const da::physics::FVehicleData& data)
{
	ms_manager.m_vehicles[id] = data;
}

void CVehicleManager::removeVehicleType(CHashString id)
{
	const std::unordered_map<CHashString, da::physics::FVehicleData>::iterator& it = ms_manager.m_vehicles.find(id);

	if (it != ms_manager.m_vehicles.end()) {
		ms_manager.m_vehicles.erase(it);
	}

}

const da::physics::FVehicleData& CVehicleManager::getVehicleData(CHashString id)
{
	return ms_manager.m_vehicles[id];
}

const std::unordered_map<CHashString, da::physics::FVehicleData>& CVehicleManager::getVehicleTypes()
{
	return ms_manager.m_vehicles;
}

CVehicleManager CVehicleManager::ms_manager;
