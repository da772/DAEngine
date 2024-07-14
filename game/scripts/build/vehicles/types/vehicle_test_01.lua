local ____lualib = require("lualib_bundle")
local __TS__New = ____lualib.__TS__New
local ____exports = {}
local ____material_data = require("daengine.graphics.material_data")
local MaterialData = ____material_data.MaterialData
local ____vector = require("daengine.vector")
local Vector2 = ____vector.Vector2
local Vector3 = ____vector.Vector3
local Vector4 = ____vector.Vector4
local ____vehicle_data = require("game.vehicle_data")
local VehicleData = ____vehicle_data.VehicleData
local WheelData = ____vehicle_data.WheelData
local VehicleTuning = ____vehicle_data.VehicleTuning
local VehicleTuningData = ____vehicle_data.VehicleTuningData
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
local vehicleTuningData = __TS__New(VehicleTuningData, tuning, wheels)
local materialData = {
    __TS__New(
        MaterialData,
        __TS__New(
            Vector4,
            1,
            0,
            0,
            1
        ),
        __TS__New(Vector3, 1, 1, 1),
        __TS__New(Vector2, 1, 1),
        0.15,
        0,
        1,
        1,
        "assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Body_DIF.dds",
        "assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Body_NRM.dds",
        "assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Body_ORM.dds",
        "assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Glass_EMS.dds"
    ),
    __TS__New(
        MaterialData,
        __TS__New(
            Vector4,
            1,
            1,
            1,
            0
        ),
        __TS__New(Vector3, 0, 0, 0),
        __TS__New(Vector2, 1, 1),
        0.15,
        0,
        1,
        1,
        "assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Glass_DIF.dds",
        "assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Glass_NRM.dds",
        "assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Glass_ORM.dds"
    ),
    __TS__New(
        MaterialData,
        __TS__New(
            Vector4,
            1,
            1,
            1,
            1
        ),
        __TS__New(Vector3, 0, 0, 0),
        __TS__New(Vector2, 1, 1),
        0.15,
        0,
        1,
        1,
        "assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Chassis_DIF.dds",
        "assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Chassis_NRM.dds",
        "assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Chassis_ORM.dds"
    ),
    __TS__New(
        MaterialData,
        __TS__New(
            Vector4,
            1,
            1,
            1,
            1
        ),
        __TS__New(Vector3, 0, 0, 0),
        __TS__New(Vector2, 1, 1),
        0.15,
        0,
        1,
        1,
        "assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Interior_DIF.dds",
        "assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Interior_NRM.dds",
        "assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Interior_ORM.dds"
    )
}
____exports.test_vehicle_01 = __TS__New(VehicleData, vehicleTuningData, materialData, "assets/prop/veh/prop_veh_sedan/prop_veh_sedan_01a.FBX")
return ____exports
