import { Vector3 } from "../daengine/vector";
import { VehicleData, WheelData, VehicleTuning } from "../game/vehicle_data";

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

export const vehicleData : VehicleData = new VehicleData(tuning, wheels);