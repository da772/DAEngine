local ____lualib = require("lualib_bundle")
local __TS__Class = ____lualib.__TS__Class
local __TS__SourceMapTraceBack = ____lualib.__TS__SourceMapTraceBack
__TS__SourceMapTraceBack(debug.getinfo(1).short_src, {["6"] = 15,["7"] = 15,["8"] = 15,["9"] = 25,["10"] = 17,["11"] = 18,["12"] = 19,["13"] = 20,["14"] = 21,["15"] = 22,["16"] = 33,["17"] = 34,["18"] = 35,["19"] = 36,["20"] = 37,["21"] = 38,["22"] = 24,["23"] = 53,["24"] = 53,["25"] = 53,["26"] = 62,["27"] = 69,["28"] = 70,["29"] = 71,["30"] = 72,["31"] = 73,["32"] = 74,["33"] = 62,["34"] = 85,["35"] = 85,["36"] = 85,["37"] = 88,["38"] = 90,["39"] = 91,["40"] = 88});
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
____exports.VehicleData = __TS__Class()
local VehicleData = ____exports.VehicleData
VehicleData.name = "VehicleData"
function VehicleData.prototype.____constructor(self, tuning, wheels)
    self.Tuning = tuning
    self.Wheels = wheels
end
return ____exports
