local ____lualib = require("lualib_bundle")
local __TS__Class = ____lualib.__TS__Class
local __TS__New = ____lualib.__TS__New
local __TS__SourceMapTraceBack = ____lualib.__TS__SourceMapTraceBack
__TS__SourceMapTraceBack(debug.getinfo(1).short_src, {["7"] = 4,["8"] = 4,["9"] = 4,["11"] = 4,["12"] = 6,["13"] = 8,["14"] = 6,["15"] = 12,["16"] = 12,["17"] = 17,["18"] = 19,["19"] = 17,["20"] = 24});
local ____exports = {}
____exports.TestBed01 = __TS__Class()
local TestBed01 = ____exports.TestBed01
TestBed01.name = "TestBed01"
function TestBed01.prototype.____constructor(self)
end
function TestBed01.prototype.initialize(self)
    print("TestBed01 Initialize")
end
function TestBed01.prototype.update(self, dt)
end
function TestBed01.prototype.shutdown(self)
    print("TestBed01 Shutdown")
end
____exports.main = __TS__New(____exports.TestBed01)
return ____exports
