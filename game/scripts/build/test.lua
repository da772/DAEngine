local ____lualib = require("lualib_bundle")
local __TS__Class = ____lualib.__TS__Class
local __TS__New = ____lualib.__TS__New
local __TS__SourceMapTraceBack = ____lualib.__TS__SourceMapTraceBack
__TS__SourceMapTraceBack(debug.getinfo(1).short_src, {["7"] = 1,["8"] = 1,["9"] = 5,["10"] = 5,["11"] = 5,["13"] = 7,["14"] = 5,["15"] = 9,["16"] = 10,["17"] = 9,["18"] = 12,["19"] = 13,["21"] = 14,["22"] = 14,["23"] = 15,["24"] = 16,["25"] = 14,["28"] = 12,["29"] = 21,["30"] = 22,["31"] = 21,["32"] = 27});
local ____exports = {}
local ____lib = require("lib")
local Employee = ____lib.Employee
____exports.TestComponent = __TS__Class()
local TestComponent = ____exports.TestComponent
TestComponent.name = "TestComponent"
function TestComponent.prototype.____constructor(self)
    self.e = __TS__New(Employee, 1, "Steve")
end
function TestComponent.prototype.initialize(self)
    print("script init")
end
function TestComponent.prototype.update(self, dt)
    local sum = 0
    do
        local i = 0
        while i < 100 do
            local ff = math.sqrt(i)
            sum = sum + ff
            i = i + 1
        end
    end
end
function TestComponent.prototype.shutdown(self)
    print("shut down")
end
____exports.component = __TS__New(____exports.TestComponent)
return ____exports
