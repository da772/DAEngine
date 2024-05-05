#pragma once

#include <daengine/core.h>
#include <daengine/physics.h>

class CVehicleManager
{
public:
	static void addVehicleType(CHashString id, const da::physics::FVehicleData& data);
	static void removeVehicleType(CHashString id);
	static const da::physics::FVehicleData& getVehicleData(CHashString id);

	static const std::unordered_map<CHashString, da::physics::FVehicleData>& getVehicleTypes();

private:
	static CVehicleManager ms_manager;

private:
	std::unordered_map<CHashString, da::physics::FVehicleData> m_vehicles;
};