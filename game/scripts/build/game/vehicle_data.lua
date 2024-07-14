local ____lualib = require("lualib_bundle")
local __TS__Class = ____lualib.__TS__Class
local ____exports = {}
____exports.VehicleTuning = __TS__Class()
local VehicleTuning = ____exports.VehicleTuning
VehicleTuning.name = "VehicleTuning"
function VehicleTuning.prototype.____constructor(self, SuspensionStiffness, SuspensionCompression, SuspensionDamping, MaxSuspensionTravelCm, FrictionSlip, MaxSuspensionForce)
    self.SuspensionStiffness = 5.88
    self.SuspensionCompression = 0.83
    self.SuspensionDamping = 0.88
    self.MaxSuspensionTravelCm = 500
    self.FrictionSlip = 10.5
    self.MaxSuspensionForce = 6000
    self.SuspensionStiffness = SuspensionStiffness
    self.SuspensionCompression = SuspensionCompression
    self.SuspensionDamping = SuspensionDamping
    self.MaxSuspensionTravelCm = MaxSuspensionTravelCm
    self.FrictionSlip = FrictionSlip
    self.MaxSuspensionForce = MaxSuspensionForce
end
____exports.WheelData = __TS__Class()
local WheelData = ____exports.WheelData
WheelData.name = "WheelData"
function WheelData.prototype.____constructor(self, wheelConnectionPoint, wheelDirection, wheelAxle, suspensionRestLength, wheelRadius, frontWheel)
    self.WheelConnectionPoint = wheelConnectionPoint
    self.WheelDirection = wheelDirection
    self.WheelAxle = wheelAxle
    self.SuspensionRestLength = suspensionRestLength
    self.WheelRadius = wheelRadius
    self.FrontWheel = frontWheel
end
____exports.VehicleTuningData = __TS__Class()
local VehicleTuningData = ____exports.VehicleTuningData
VehicleTuningData.name = "VehicleTuningData"
function VehicleTuningData.prototype.____constructor(self, tuning, wheels)
    self.Tuning = tuning
    self.Wheels = wheels
end
____exports.VehicleData = __TS__Class()
local VehicleData = ____exports.VehicleData
VehicleData.name = "VehicleData"
function VehicleData.prototype.____constructor(self, tuningData, materialData, vehicleMesh)
    self.TuningData = tuningData
    self.Materials = materialData
    self.VehicleMesh = vehicleMesh
end
____exports.VehicleManager = {}
local VehicleManager = ____exports.VehicleManager
do
    function VehicleManager.RegisterVehicle(self, id, data)
        register_vehicle(nil, id, data)
    end
    function VehicleManager.UnregisterVehicle(self, id, cleanup)
        unregister_vehicle(nil, id, cleanup)
    end
end
return ____exports
