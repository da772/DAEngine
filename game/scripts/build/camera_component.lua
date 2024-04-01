local ____lualib = require("lualib_bundle")
local __TS__Class = ____lualib.__TS__Class
local __TS__ClassExtends = ____lualib.__TS__ClassExtends
local __TS__New = ____lualib.__TS__New
local __TS__SourceMapTraceBack = ____lualib.__TS__SourceMapTraceBack
__TS__SourceMapTraceBack(debug.getinfo(1).short_src, {["8"] = 2,["9"] = 2,["10"] = 3,["11"] = 3,["12"] = 4,["13"] = 4,["14"] = 5,["15"] = 5,["16"] = 6,["17"] = 6,["18"] = 7,["19"] = 7,["20"] = 8,["21"] = 8,["22"] = 11,["23"] = 11,["24"] = 11,["25"] = 11,["27"] = 11,["28"] = 13,["29"] = 14,["30"] = 15,["31"] = 11,["32"] = 17,["33"] = 18,["34"] = 19,["35"] = 17,["36"] = 21,["37"] = 22,["38"] = 24,["39"] = 26,["40"] = 28,["42"] = 21,["43"] = 33,["44"] = 34,["45"] = 36,["46"] = 37,["48"] = 40,["49"] = 42,["51"] = 45,["52"] = 47,["54"] = 50,["55"] = 52,["57"] = 55,["58"] = 57,["60"] = 60,["61"] = 62,["63"] = 65,["64"] = 67,["65"] = 68,["66"] = 70,["69"] = 33,["70"] = 75,["71"] = 77,["72"] = 78,["73"] = 79,["74"] = 80,["76"] = 83,["77"] = 75,["78"] = 87,["79"] = 88,["80"] = 89,["81"] = 87,["82"] = 94});
local ____exports = {}
local ____component = require("daengine.component")
local NativeComponent = ____component.NativeComponent
local ____input = require("daengine.input")
local Input = ____input.Input
local ____camera = require("daengine.camera")
local Camera = ____camera.Camera
local ____vector = require("daengine.vector")
local Vector2 = ____vector.Vector2
local ____input_enum = require("daengine.input_enum")
local Inputs = ____input_enum.Inputs
local ____debug = require("daengine.debug")
local Debug = ____debug.Debug
local ____imgui = require("daengine.imgui")
local ImGui = ____imgui.ImGui
____exports.CameraComponent = __TS__Class()
local CameraComponent = ____exports.CameraComponent
CameraComponent.name = "CameraComponent"
__TS__ClassExtends(CameraComponent, NativeComponent)
function CameraComponent.prototype.____constructor(self, ...)
    NativeComponent.prototype.____constructor(self, ...)
    self.cursorPos = __TS__New(Vector2)
    self.camSpeed = 5
    self.velSpeed = 5000
end
function CameraComponent.prototype.initialize(self)
    print("camera component init")
    Debug:RegisterDebugMenu("CameraComponent", self, ____exports.CameraComponent.prototype.debugUpdate)
end
function CameraComponent.prototype.update(self, dt)
    self:cameraInput(dt)
    local pos = Input:CursorPos()
    if not pos:equals(self.cursorPos) then
        self.cursorPos = pos
    end
end
function CameraComponent.prototype.cameraInput(self, dt)
    if Input:KeyPressed(Inputs.KEY_UP) then
        _break()
        Camera:Move(Camera:GetForward():mul(self.camSpeed):mul(dt))
    end
    if Input:KeyPressed(Inputs.KEY_DOWN) then
        Camera:Move(Camera:GetForward():neg():mul(self.camSpeed):mul(dt))
    end
    if Input:KeyPressed(Inputs.KEY_LEFT) then
        Camera:Move(Camera:GetRight():neg():mul(self.camSpeed):mul(dt))
    end
    if Input:KeyPressed(Inputs.KEY_RIGHT) then
        Camera:Move(Camera:GetRight():mul(self.camSpeed):mul(dt))
    end
    if Input:KeyPressed(Inputs.KEY_E) then
        Camera:Move(Camera:GetUp():neg():mul(self.camSpeed):mul(dt))
    end
    if Input:KeyPressed(Inputs.KEY_Q) then
        Camera:Move(Camera:GetUp():mul(self.camSpeed):mul(dt))
    end
    if Input:MousePressed(Inputs.MOUSE_BUTTON_2) then
        local pos = Input:CursorPos()
        if self.cursorPos.x >= 0 and self.cursorPos.y >= 0 then
            Camera:Rotate(__TS__New(Vector2, self.cursorPos.y - pos.y, self.cursorPos.x - pos.x):mul(180 / 600))
        end
    end
end
function CameraComponent.prototype.debugUpdate(self)
    if ImGui:Begin("Camera Component") then
        ImGui:LabelText("Camera Speed")
        ImGui:SameLine()
        self.camSpeed = ImGui:InputFloat("##CamSpeed", self.camSpeed)
    end
    ImGui:End()
end
function CameraComponent.prototype.shutdown(self)
    print("camera component shutdown")
    Debug:UnregisterDebugMenu("CameraComponent")
end
____exports.component = __TS__New(____exports.CameraComponent)
return ____exports
