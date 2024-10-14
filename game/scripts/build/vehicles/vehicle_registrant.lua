local ____lualib = require("lualib_bundle")
local __TS__SourceMapTraceBack = ____lualib.__TS__SourceMapTraceBack
__TS__SourceMapTraceBack(debug.getinfo(1).short_src, {["5"] = 1,["6"] = 1,["7"] = 6,["9"] = 10,["11"] = 10,["12"] = 13,["13"] = 14,["14"] = 15,["15"] = 16,["17"] = 12,["18"] = 10,["19"] = 21,["20"] = 22,["21"] = 23,["23"] = 20});
local ____exports = {}
local ____vehicle_data = require("game.vehicle_data")
local VehicleManager = ____vehicle_data.VehicleManager
local vehicle_types = {}
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
