import { VehicleData, VehicleManager } from "../game/vehicle_data";

// Vehicles
import { test_vehicle_01 } from "./types/vehicle_test_01";

const vehicle_types : {[id : string] : VehicleData} = {
    "test_vehicle_01" : test_vehicle_01
}

export namespace VehicleRegistrant
{
    export function register() {
        for (const key in vehicle_types) {
            const value = vehicle_types[key];
            VehicleManager.RegisterVehicle(key, value);   
            print(`Registered Vehicle: ${key}`);
        }
    }
    
    export function unregister() {
        for (const key in vehicle_types) {
            VehicleManager.UnregisterVehicle(key);
            print(`Unregistered Vehicle: ${key}`);
        }
    }
}

