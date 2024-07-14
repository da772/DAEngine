local ____lualib = require("lualib_bundle")
local __TS__Class = ____lualib.__TS__Class
local __TS__New = ____lualib.__TS__New
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
