import { MaterialData } from "../../daengine/graphics/material_data";
import { Vector2, Vector3, Vector4 } from "../../daengine/vector";
import { VehicleData, WheelData, VehicleTuning, VehicleTuningData } from "../../game/vehicle_data";

const tuning : VehicleTuning = new VehicleTuning( 5.88, 0.83, 0.88, 500., 10.5, 6000.0);
const wheels : WheelData[] =  [
    // FR
    new WheelData(new Vector3(0.8,  1.468, -0.37).mul(new Vector3(1.,1.,1.)), new Vector3(0,0,-1), new Vector3(1,0,0),  0.075, 0.353, true),
    // FL
    new WheelData(new Vector3(0.8,  1.468, -0.37).mul(new Vector3(-1.,1.,1.)), new Vector3(0,0,-1), new Vector3(1,0,0),  0.075, 0.353, true),
    // BR
    new WheelData(new Vector3(0.8,  1.468, -0.37).mul(new Vector3(1.,-1.,1.)), new Vector3(0,0,-1), new Vector3(1,0,0),  0.075, 0.353, false),
    // BL
    new WheelData(new Vector3(0.8,  1.468, -0.37).mul(new Vector3(-1.,-1.,1.)), new Vector3(0,0,-1), new Vector3(1,0,0),  0.075, 0.353, false),
]

const vehicleTuningData : VehicleTuningData = new VehicleTuningData(tuning, wheels);

const materialData : MaterialData[] = [
    // Material 0
    new MaterialData(new Vector4(1.0,0.0,0.0,1.0)
    , new Vector3(1.0,1.0,1.0)
    , new Vector2(1.0,1.0)
    , 0.15, 0.0, 1.0, 1.0
    , "assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Body_DIF.dds"
    , "assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Body_NRM.dds"
    , ""
    , "assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Glass_EMS.dds"),
    // Material 1
    new MaterialData(new Vector4(1.0,1.0,1.0,0.0)
    , new Vector3(0.0,0.0,0.0)
    , new Vector2(1.0,1.0)
    , 0.15, 0.0, 1.0, 1.0
    , "assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Glass_DIF.dds"
    , "assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Glass_NRM.dds"
    , "assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Glass_ORM.dds"),
    // Material 2
    new MaterialData(new Vector4(1.0,1.0,1.0,1.0)
    , new Vector3(0.0,0.0,0.0)
    , new Vector2(1.0,1.0)
    , 0.15, 0.0, 1.0, 1.0
    , "assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Chassis_DIF.dds"
    , "assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Chassis_NRM.dds"
    , "assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Chassis_ORM.dds"),
    // Material 3
    new MaterialData(new Vector4(1.0,1.0,1.0,1.0)
    , new Vector3(0.0,0.0,0.0)
    , new Vector2(1.0,1.0)
    , 0.15, 0.0, 1.0, 1.0
    , "assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Interior_DIF.dds"
    , "assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Interior_NRM.dds"
    , "assets/textures/veh/prop_veh_sedan_01a/Tex_Veh_Sedan_Interior_ORM.dds")
];

export const test_vehicle_01 : VehicleData = new VehicleData(vehicleTuningData, materialData, "assets/prop/veh/prop_veh_sedan/prop_veh_sedan_01a.FBX");