local ____lualib = require("lualib_bundle")
local __TS__New = ____lualib.__TS__New
local __TS__SourceMapTraceBack = ____lualib.__TS__SourceMapTraceBack
__TS__SourceMapTraceBack(debug.getinfo(1).short_src, {["6"] = 1,["7"] = 1,["8"] = 2,["9"] = 2,["10"] = 2,["11"] = 2,["12"] = 3,["13"] = 3,["14"] = 3,["15"] = 3,["16"] = 3,["17"] = 5,["18"] = 5,["19"] = 5,["20"] = 5,["21"] = 5,["22"] = 5,["23"] = 5,["24"] = 5,["25"] = 5,["26"] = 6,["27"] = 8,["28"] = 8,["29"] = 8,["30"] = 8,["31"] = 8,["32"] = 8,["33"] = 8,["34"] = 8,["35"] = 6,["36"] = 10,["37"] = 10,["38"] = 10,["39"] = 10,["40"] = 10,["41"] = 10,["42"] = 10,["43"] = 10,["44"] = 6,["45"] = 12,["46"] = 12,["47"] = 12,["48"] = 12,["49"] = 12,["50"] = 12,["51"] = 12,["52"] = 12,["53"] = 6,["54"] = 14,["55"] = 14,["56"] = 14,["57"] = 14,["58"] = 14,["59"] = 14,["60"] = 14,["61"] = 14,["62"] = 6,["63"] = 6,["64"] = 17,["65"] = 19,["66"] = 21,["67"] = 21,["68"] = 21,["69"] = 21,["70"] = 21,["71"] = 21,["72"] = 21,["73"] = 21,["74"] = 21,["75"] = 21,["76"] = 21,["77"] = 21,["78"] = 21,["79"] = 21,["80"] = 21,["81"] = 21,["82"] = 21,["83"] = 21,["84"] = 21,["85"] = 19,["86"] = 31,["87"] = 31,["88"] = 31,["89"] = 31,["90"] = 31,["91"] = 31,["92"] = 31,["93"] = 31,["94"] = 31,["95"] = 31,["96"] = 31,["97"] = 31,["98"] = 31,["99"] = 31,["100"] = 31,["101"] = 31,["102"] = 31,["103"] = 31,["104"] = 19,["105"] = 40,["106"] = 40,["107"] = 40,["108"] = 40,["109"] = 40,["110"] = 40,["111"] = 40,["112"] = 40,["113"] = 40,["114"] = 40,["115"] = 40,["116"] = 40,["117"] = 40,["118"] = 40,["119"] = 40,["120"] = 40,["121"] = 40,["122"] = 40,["123"] = 19,["124"] = 49,["125"] = 49,["126"] = 49,["127"] = 49,["128"] = 49,["129"] = 49,["130"] = 49,["131"] = 49,["132"] = 49,["133"] = 49,["134"] = 49,["135"] = 49,["136"] = 49,["137"] = 49,["138"] = 49,["139"] = 49,["140"] = 49,["141"] = 49,["142"] = 19,["143"] = 19,["144"] = 61});
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
