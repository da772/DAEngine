local ____lualib = require("lualib_bundle")
local __TS__Class = ____lualib.__TS__Class
local __TS__SourceMapTraceBack = ____lualib.__TS__SourceMapTraceBack
__TS__SourceMapTraceBack(debug.getinfo(1).short_src, {["6"] = 10,["7"] = 10,["8"] = 10,["9"] = 13,["10"] = 14,["11"] = 13});
local ____exports = {}
____exports.VehicleData = __TS__Class()
local VehicleData = ____exports.VehicleData
VehicleData.name = "VehicleData"
function VehicleData.prototype.____constructor(self, wheels)
    self.Wheels = wheels
end
return ____exports
