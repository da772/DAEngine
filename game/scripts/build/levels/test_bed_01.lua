local ____lualib = require("lualib_bundle")
local __TS__Class = ____lualib.__TS__Class
local __TS__New = ____lualib.__TS__New
local __TS__SourceMapTraceBack = ____lualib.__TS__SourceMapTraceBack
__TS__SourceMapTraceBack(debug.getinfo(1).short_src, {["7"] = 2,["8"] = 2,["9"] = 4,["10"] = 4,["11"] = 4,["13"] = 4,["14"] = 6,["15"] = 8,["16"] = 9,["17"] = 6,["18"] = 12,["19"] = 12,["20"] = 17,["21"] = 19,["22"] = 20,["23"] = 17,["24"] = 24});
local ____exports = {}
local ____vehicle_registrant = require("vehicles.vehicle_registrant")
local VehicleRegistrant = ____vehicle_registrant.VehicleRegistrant
____exports.TestBed01 = __TS__Class()
local TestBed01 = ____exports.TestBed01
TestBed01.name = "TestBed01"
function TestBed01.prototype.____constructor(self)
end
function TestBed01.prototype.initialize(self)
    print("TestBed01 Initialize")
    VehicleRegistrant:register()
end
function TestBed01.prototype.update(self, dt)
end
function TestBed01.prototype.shutdown(self)
    print("TestBed01 Shutdown")
    VehicleRegistrant:unregister()
end
____exports.main = __TS__New(____exports.TestBed01)
return ____exports
