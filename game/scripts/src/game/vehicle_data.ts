import { Vector3 } from "../daengine/vector";

/*
struct FVehicleTuning
{
	float SuspensionStiffness = 5.88f;
	float SuspensionCompression = 0.83f;
	float SuspensionDamping = 0.88f;
	float MaxSuspensionTravelCm = 500.f;
	float FrictionSlip = 10.5f;
	float MaxSuspensionForce = 6000.f;
};
*/

export class VehicleTuning
{
    public SuspensionStiffness : number = 5.88;
    public SuspensionCompression : number = 0.83;
    public SuspensionDamping : number = 0.88;
    public MaxSuspensionTravelCm : number = 500.;
    public FrictionSlip : number = 10.5;
    public MaxSuspensionForce : number = 6000.;

    constructor (
        SuspensionStiffness : number,
        SuspensionCompression : number,
        SuspensionDamping : number,
        MaxSuspensionTravelCm : number,
        FrictionSlip : number,
        MaxSuspensionForce : number,
    ) {
        
        this.SuspensionStiffness = SuspensionStiffness;
        this.SuspensionCompression = SuspensionCompression;
        this.SuspensionDamping = SuspensionDamping;
        this.MaxSuspensionTravelCm = MaxSuspensionTravelCm;
        this.FrictionSlip = FrictionSlip;
        this.MaxSuspensionForce = MaxSuspensionForce;
    }
}

/*
struct FWheelData
{
	glm::vec3 WheelConnectionPoint;
	glm::vec3 WheelDirection;
	glm::vec3 WheelAxle;
	float SuspensionRestLength;
	float WheelRadius;
	bool FrontWheel;
};
*/
export class WheelData
{
    public WheelConnectionPoint : Vector3
    public WheelDirection : Vector3
    public WheelAxle : Vector3
    public SuspensionRestLength : number;
    public WheelRadius : number;
    public FrontWheel : boolean;

    constructor(wheelConnectionPoint : Vector3
        , wheelDirection : Vector3
        , wheelAxle : Vector3
        , suspensionRestLength : number
        , wheelRadius : number
        , frontWheel : boolean)
    {
        this.WheelConnectionPoint = wheelConnectionPoint;
        this.WheelDirection = wheelDirection;
        this.WheelAxle = wheelAxle;
        this.SuspensionRestLength = suspensionRestLength;
        this.WheelRadius = wheelRadius;
        this.FrontWheel = frontWheel;
    }
}

/*
struct FScriptVehicleData
{
	FVehicleTuning Tuning;
	std::vector<FWheelData> Wheels;
};
*/
export class VehicleData {
    public Tuning : VehicleTuning;
    public Wheels : WheelData[];
    public constructor(tuning: VehicleTuning, wheels : WheelData[]) 
    {
        this.Tuning = tuning;
        this.Wheels = wheels;
    }
}