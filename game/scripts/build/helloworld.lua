local ____lualib = require("lualib_bundle")
local __TS__Class = ____lualib.__TS__Class
local __TS__ClassExtends = ____lualib.__TS__ClassExtends
local __TS__New = ____lualib.__TS__New
local __TS__SourceMapTraceBack = ____lualib.__TS__SourceMapTraceBack
__TS__SourceMapTraceBack(debug.getinfo(1).short_src, {["8"] = 1,["9"] = 1,["10"] = 3,["11"] = 3,["12"] = 4,["13"] = 4,["14"] = 5,["15"] = 5,["16"] = 6,["17"] = 6,["18"] = 7,["19"] = 7,["20"] = 8,["21"] = 8,["22"] = 11,["23"] = 11,["24"] = 11,["25"] = 11,["27"] = 11,["28"] = 13,["29"] = 11,["30"] = 15,["31"] = 16,["32"] = 17,["33"] = 15,["34"] = 20,["35"] = 21,["36"] = 23,["37"] = 24,["39"] = 27,["40"] = 29,["41"] = 30,["43"] = 20,["44"] = 34,["45"] = 36,["46"] = 37,["47"] = 38,["48"] = 39,["49"] = 40,["50"] = 41,["51"] = 42,["52"] = 42,["55"] = 47,["56"] = 34,["57"] = 50,["58"] = 51,["59"] = 52,["60"] = 50,["61"] = 57});
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
    print("script init")
    Debug:RegisterDebugMenu("MyComponent", self, ____exports.MyComponent.prototype.debugUpdate)
end
function MyComponent.prototype.update(self, dt)
    if Input:KeyPressed(Inputs.KEY_T) then
        local pos = self:GetEntity():getPosition()
        self:GetEntity():setPosition(__TS__New(Vector3, pos.x, pos.y, pos.z + 1.61 * dt))
    end
    if Input:KeyPressed(Inputs.KEY_G) then
        local pos = self:GetEntity():getPosition()
        self:GetEntity():setPosition(__TS__New(Vector3, pos.x, pos.y, pos.z - 1.61 * dt))
    end
end
function MyComponent.prototype.debugUpdate(self)
    if ImGui:Begin("Hello World!") then
        ImGui:LabelText((("employee is sick " .. self.e.empName) .. " is sick: ") .. tostring(self.e.empCode))
        ImGui:LabelText("Hello people")
        ImGui:LabelText("This is my guid: " .. self.Id.Index)
        ImGui:LabelText("This is my parent guid: " .. self.Entity:GetId().Index)
        if ImGui:Button("Press Me") then
            local ____self_e_0, ____empCode_1 = self.e, "empCode"
            ____self_e_0[____empCode_1] = ____self_e_0[____empCode_1] + 1
        end
    end
    ImGui:End()
end
function MyComponent.prototype.shutdown(self)
    print("hello world shut down")
    Debug:UnregisterDebugMenu("MyComponent")
end
____exports.component = __TS__New(____exports.MyComponent)
return ____exports
