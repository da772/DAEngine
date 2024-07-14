--[[ Generated with https://github.com/TypeScriptToLua/TypeScriptToLua ]]
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
