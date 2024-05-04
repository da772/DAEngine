local ____lualib = require("lualib_bundle")
local __TS__New = ____lualib.__TS__New
local __TS__SourceMapTraceBack = ____lualib.__TS__SourceMapTraceBack
__TS__SourceMapTraceBack(debug.getinfo(1).short_src, {["6"] = 1,["7"] = 1,["8"] = 2,["9"] = 2,["10"] = 2,["11"] = 2,["12"] = 4,["13"] = 4,["14"] = 4,["15"] = 4,["16"] = 4,["17"] = 4,["18"] = 4,["19"] = 4,["20"] = 4,["21"] = 6,["22"] = 8,["23"] = 8,["24"] = 8,["25"] = 8,["26"] = 8,["27"] = 8,["28"] = 8,["29"] = 8,["30"] = 6,["31"] = 10,["32"] = 10,["33"] = 10,["34"] = 10,["35"] = 10,["36"] = 10,["37"] = 10,["38"] = 10,["39"] = 6,["40"] = 12,["41"] = 12,["42"] = 12,["43"] = 12,["44"] = 12,["45"] = 12,["46"] = 12,["47"] = 12,["48"] = 6,["49"] = 14,["50"] = 14,["51"] = 14,["52"] = 14,["53"] = 14,["54"] = 14,["55"] = 14,["56"] = 14,["57"] = 6,["58"] = 6,["59"] = 17});
local ____exports = {}
local ____vector = require("daengine.vector")
local Vector3 = ____vector.Vector3
local ____vehicle_data = require("game.vehicle_data")
local VehicleData = ____vehicle_data.VehicleData
local WheelData = ____vehicle_data.WheelData
local VehicleTuning = ____vehicle_data.VehicleTuning
local tuning = __TS__New(
    VehicleTuning,
    5.88,
    0.83,
    0.88,
    500,
    10.5,
    6000
)
local wheels = {
    __TS__New(
        WheelData,
        __TS__New(Vector3, 0.8, 1.468, -0.37):mul(__TS__New(Vector3, 1, 1, 1)),
        __TS__New(Vector3, 0, 0, -1),
        __TS__New(Vector3, 1, 0, 0),
        0.075,
        0.353,
        true
    ),
    __TS__New(
        WheelData,
        __TS__New(Vector3, 0.8, 1.468, -0.37):mul(__TS__New(Vector3, -1, 1, 1)),
        __TS__New(Vector3, 0, 0, -1),
        __TS__New(Vector3, 1, 0, 0),
        0.075,
        0.353,
        true
    ),
    __TS__New(
        WheelData,
        __TS__New(Vector3, 0.8, 1.468, -0.37):mul(__TS__New(Vector3, 1, -1, 1)),
        __TS__New(Vector3, 0, 0, -1),
        __TS__New(Vector3, 1, 0, 0),
        0.075,
        0.353,
        false
    ),
    __TS__New(
        WheelData,
        __TS__New(Vector3, 0.8, 1.468, -0.37):mul(__TS__New(Vector3, -1, -1, 1)),
        __TS__New(Vector3, 0, 0, -1),
        __TS__New(Vector3, 1, 0, 0),
        0.075,
        0.353,
        false
    )
}
____exports.vehicleData = __TS__New(VehicleData, tuning, wheels)
return ____exports
