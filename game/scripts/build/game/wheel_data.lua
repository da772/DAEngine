local ____lualib = require("lualib_bundle")
local __TS__Class = ____lualib.__TS__Class
local __TS__SourceMapTraceBack = ____lualib.__TS__SourceMapTraceBack
__TS__SourceMapTraceBack(debug.getinfo(1).short_src, {["6"] = 13,["7"] = 13,["8"] = 13,["9"] = 21,["10"] = 23,["11"] = 24,["12"] = 25,["13"] = 26,["14"] = 27,["15"] = 21});
local ____exports = {}
____exports.WheelData = __TS__Class()
local WheelData = ____exports.WheelData
WheelData.name = "WheelData"
function WheelData.prototype.____constructor(self, wheelConnectionPoint, wheelDirection, wheelAxle, suspensionRestLength, wheelRadius)
    self.WheelConnectionPoint = wheelConnectionPoint
    self.WheelDirection = wheelDirection
    self.WheelAxle = wheelAxle
    self.SuspensionRestLength = suspensionRestLength
    self.WheelRadius = wheelRadius
end
return ____exports
