local ____lualib = require("lualib_bundle")
local __TS__New = ____lualib.__TS__New
local __TS__SourceMapTraceBack = ____lualib.__TS__SourceMapTraceBack
__TS__SourceMapTraceBack(debug.getinfo(1).short_src, {["6"] = 1,["7"] = 1,["8"] = 2,["9"] = 2,["10"] = 2,["11"] = 2,["12"] = 3,["13"] = 3,["14"] = 3,["15"] = 3,["16"] = 3,["17"] = 5,["18"] = 5,["19"] = 5,["20"] = 5,["21"] = 5,["22"] = 5,["23"] = 5,["24"] = 5,["25"] = 5,["26"] = 6,["27"] = 8,["28"] = 8,["29"] = 8,["30"] = 8,["31"] = 8,["32"] = 8,["33"] = 8,["34"] = 8,["35"] = 6,["36"] = 10,["37"] = 10,["38"] = 10,["39"] = 10,["40"] = 10,["41"] = 10,["42"] = 10,["43"] = 10,["44"] = 6,["45"] = 12,["46"] = 12,["47"] = 12,["48"] = 12,["49"] = 12,["50"] = 12,["51"] = 12,["52"] = 12,["53"] = 6,["54"] = 14,["55"] = 14,["56"] = 14,["57"] = 14,["58"] = 14,["59"] = 14,["60"] = 14,["61"] = 14,["62"] = 6,["63"] = 6,["64"] = 17,["65"] = 19,["66"] = 21,["67"] = 21,["68"] = 21,["69"] = 21,["70"] = 21,["71"] = 21,["72"] = 21,["73"] = 21,["74"] = 21,["75"] = 21,["76"] = 21,["77"] = 21,["78"] = 21,["79"] = 21,["80"] = 21,["81"] = 21,["82"] = 21,["83"] = 21,["84"] = 21,["85"] = 19,["86"] = 30,["87"] = 30,["88"] = 30,["89"] = 30,["90"] = 30,["91"] = 30,["92"] = 30,["93"] = 30,["94"] = 30,["95"] = 30,["96"] = 30,["97"] = 30,["98"] = 30,["99"] = 30,["100"] = 30,["101"] = 30,["102"] = 30,["103"] = 30,["104"] = 19,["105"] = 38,["106"] = 38,["107"] = 38,["108"] = 38,["109"] = 38,["110"] = 38,["111"] = 38,["112"] = 38,["113"] = 38,["114"] = 38,["115"] = 38,["116"] = 38,["117"] = 38,["118"] = 38,["119"] = 38,["120"] = 38,["121"] = 38,["122"] = 38,["123"] = 19,["124"] = 46,["125"] = 46,["126"] = 46,["127"] = 46,["128"] = 46,["129"] = 46,["130"] = 46,["131"] = 46,["132"] = 46,["133"] = 46,["134"] = 46,["135"] = 46,["136"] = 46,["137"] = 46,["138"] = 46,["139"] = 46,["140"] = 46,["141"] = 46,["142"] = 19,["143"] = 19,["144"] = 55});
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
        "",
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
