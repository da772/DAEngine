local ____lualib = require("lualib_bundle")
local __TS__Class = ____lualib.__TS__Class
local __TS__ClassExtends = ____lualib.__TS__ClassExtends
local __TS__New = ____lualib.__TS__New
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
    print(nil, "camera component init")
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
    if Input:KeyPressed(Inputs.KEY_W) then
        Camera:Move(Camera:GetForward():mul(self.camSpeed):mul(dt))
    end
    if Input:KeyPressed(Inputs.KEY_S) then
        Camera:Move(Camera:GetForward():neg():mul(self.camSpeed):mul(dt))
    end
    if Input:KeyPressed(Inputs.KEY_A) then
        Camera:Move(Camera:GetRight():neg():mul(self.camSpeed):mul(dt))
    end
    if Input:KeyPressed(Inputs.KEY_D) then
        Camera:Move(Camera:GetRight():mul(self.camSpeed):mul(dt))
    end
    if Input:KeyPressed(Inputs.KEY_SPACE) then
        Camera:Move(Camera:GetUp():mul(self.camSpeed):mul(dt))
    end
    if Input:KeyPressed(Inputs.KEY_E) then
        Camera:Move(Camera:GetUp():neg():mul(self.camSpeed):mul(dt))
    end
    if Input:KeyPressed(Inputs.KEY_Q) then
        Camera:Move(Camera:GetUp():mul(self.camSpeed):mul(dt))
    end
    if Input:KeyPressed(Inputs.KEY_F) then
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
    print(nil, "camera component")
    Debug:UnregisterDebugMenu("CameraComponent")
end
____exports.component = __TS__New(____exports.CameraComponent)
return ____exports
