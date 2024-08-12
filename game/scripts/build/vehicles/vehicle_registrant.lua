local ____lualib = require("lualib_bundle")
local __TS__SourceMapTraceBack = ____lualib.__TS__SourceMapTraceBack
__TS__SourceMapTraceBack(debug.getinfo(1).short_src, {["5"] = 1,["6"] = 1,["7"] = 4,["8"] = 4,["9"] = 6,["11"] = 10,["13"] = 10,["14"] = 13,["15"] = 14,["16"] = 15,["17"] = 16,["19"] = 12,["20"] = 10,["21"] = 21,["22"] = 22,["23"] = 23,["25"] = 20});
local ____exports = {}
local ____vehicle_data = require("game.vehicle_data")
local VehicleManager = ____vehicle_data.VehicleManager
local ____vehicle_test_01 = require("vehicles.types.vehicle_test_01")
local test_vehicle_01 = ____vehicle_test_01.test_vehicle_01
local vehicle_types = {test_vehicle_01 = test_vehicle_01}
____exports.VehicleRegistrant = {}
local VehicleRegistrant = ____exports.VehicleRegistrant
do
    function VehicleRegistrant.register(self)
        for key in pairs(vehicle_types) do
            local value = vehicle_types[key]
            VehicleManager:RegisterVehicle(key, value)
            print("Registered Vehicle: " .. key)
        end
    end
    function VehicleRegistrant.unregister(self)
        for key in pairs(vehicle_types) do
            VehicleManager:UnregisterVehicle(key, true)
            print("Unregistered Vehicle: " .. key)
        end
    end
end
return ____exports
