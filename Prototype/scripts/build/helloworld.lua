local ____lualib = require("lualib_bundle")
local __TS__Class = ____lualib.__TS__Class
local __TS__New = ____lualib.__TS__New
local ____exports = {}
local ____lib = require("lib")
local Employee = ____lib.Employee
local ____imgui = require("core.imgui")
local ImGui = ____imgui.ImGui
____exports.MyComponent = __TS__Class()
local MyComponent = ____exports.MyComponent
MyComponent.name = "MyComponent"
function MyComponent.prototype.____constructor(self)
    self.e = __TS__New(Employee, 1, "Steve")
end
function MyComponent.prototype.initialize(self)
    print(nil, "script init")
end
function MyComponent.prototype.update(self, dt)
    if ImGui:Begin("Hello World!") then
        ImGui:LabelText(
            "employee is sick",
            (self.e.empName .. " is sick: ") .. tostring(self.e.empCode)
        )
        ImGui:LabelText("###end", "Hello people")
        if ImGui:Button("Press Me") then
            local ____self_e_0, ____empCode_1 = self.e, "empCode"
            ____self_e_0[____empCode_1] = ____self_e_0[____empCode_1] + 1
        end
    end
    ImGui:End()
end
function MyComponent.prototype.shutdown(self)
    print(nil, "shut down")
end
____exports.component = __TS__New(____exports.MyComponent)
return ____exports
