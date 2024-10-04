
#include "vehicle_manager.h"

void CVehicleManager::addVehicleType(CHashString id, const da::FVehicleData& data)
{
	ms_manager.m_vehicles[id] = data;
}

void CVehicleManager::removeVehicleType(CHashString id, bool cleanup)
{
	const std::unordered_map<CHashString, da::FVehicleData>::iterator& it = ms_manager.m_vehicles.find(id);

	if (it != ms_manager.m_vehicles.end()) {
		if (cleanup) {
			for (da::FMaterialData& mat : it->second.Materials) {
				mat.cleanup();
			}
		}
		ms_manager.m_vehicles.erase(it);
	}

}

const da::FVehicleData& CVehicleManager::getVehicleData(CHashString id)
{
	return ms_manager.m_vehicles[id];
}

const std::unordered_map<CHashString, da::FVehicleData>& CVehicleManager::getVehicleTypes()
{
	return ms_manager.m_vehicles;
}

CVehicleManager CVehicleManager::ms_manager;
