import { Level } from "../daengine/level";
import { VehicleRegistrant } from "../vehicles/vehicle_registrant";

export class TestBed01 implements Level 
{
    initialize(): void 
    {
        print("TestBed01 Initialize");
        //VehicleRegistrant.register();
    }

    update(dt: number): void 
    {
        
    }
    
    shutdown(): void 
    {
        print("TestBed01 Shutdown");
        //VehicleRegistrant.unregister();
    }
}

export let main : TestBed01 = new TestBed01();