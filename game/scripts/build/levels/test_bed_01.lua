local ____lualib = require("lualib_bundle")
local __TS__Class = ____lualib.__TS__Class
local __TS__New = ____lualib.__TS__New
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
