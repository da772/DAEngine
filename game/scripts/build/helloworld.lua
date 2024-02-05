local ____lualib = require("lualib_bundle")
local __TS__Class = ____lualib.__TS__Class
local __TS__ClassExtends = ____lualib.__TS__ClassExtends
local __TS__New = ____lualib.__TS__New
local ____exports = {}
local ____lib = require("lib")
local Employee = ____lib.Employee
local ____imgui = require("daengine.imgui")
local ImGui = ____imgui.ImGui
local ____component = require("daengine.component")
local NativeComponent = ____component.NativeComponent
local ____debug = require("daengine.debug")
local Debug = ____debug.Debug
local ____input = require("daengine.input")
local Input = ____input.Input
local ____vector = require("daengine.vector")
local Vector3 = ____vector.Vector3
local ____input_enum = require("daengine.input_enum")
local Inputs = ____input_enum.Inputs
____exports.MyComponent = __TS__Class()
local MyComponent = ____exports.MyComponent
MyComponent.name = "MyComponent"
__TS__ClassExtends(MyComponent, NativeComponent)
function MyComponent.prototype.____constructor(self, ...)
    NativeComponent.prototype.____constructor(self, ...)
    self.e = __TS__New(Employee, 1, "Steve")
end
function MyComponent.prototype.initialize(self)
    print(nil, "script init")
    Debug:RegisterDebugMenu("MyComponent", self, ____exports.MyComponent.prototype.debugUpdate)
end
function MyComponent.prototype.update(self, dt)
    if Input:KeyPressed(Inputs.KEY_T) then
        local pos = self:GetEntity():getPosition()
        self:GetEntity():setPosition(__TS__New(Vector3, pos.x, pos.y, pos.z + 1.61))
    end
end
function MyComponent.prototype.debugUpdate(self)
    if ImGui:Begin("Hello World!") then
        ImGui:LabelText((("employee is sick " .. self.e.empName) .. " is sick: ") .. tostring(self.e.empCode))
        ImGui:LabelText("Hello people")
        if ImGui:Button("Press Me") then
            local ____self_e_0, ____empCode_1 = self.e, "empCode"
            ____self_e_0[____empCode_1] = ____self_e_0[____empCode_1] + 1
        end
    end
    ImGui:End()
end
function MyComponent.prototype.shutdown(self)
    print(nil, "shut down")
    Debug:UnregisterDebugMenu("MyComponent")
end
____exports.component = __TS__New(____exports.MyComponent)
return ____exports
