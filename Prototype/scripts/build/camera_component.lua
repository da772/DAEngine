local ____lualib = require("lualib_bundle")
local __TS__Class = ____lualib.__TS__Class
local __TS__New = ____lualib.__TS__New
local ____exports = {}
local ____input = require("core.input")
local Input = ____input.Input
local ____camera = require("core.camera")
local Camera = ____camera.Camera
local ____vector = require("core.vector")
local Vector2 = ____vector.Vector2
local ____input_enum = require("core.input_enum")
local Inputs = ____input_enum.Inputs
____exports.CameraComponent = __TS__Class()
local CameraComponent = ____exports.CameraComponent
CameraComponent.name = "CameraComponent"
function CameraComponent.prototype.____constructor(self)
    self.cursorPos = __TS__New(Vector2)
    self.camSpeed = 0.1
end
function CameraComponent.prototype.initialize(self)
    print(nil, "camera component init")
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
    if Input:KeyPressed(Inputs.KEY_LEFT_CONTROL) then
        Camera:Move(Camera:GetUp():neg():mul(self.camSpeed):mul(dt))
    end
    if Input:KeyPressed(Inputs.KEY_LEFT_SHIFT) then
        Camera:Move(Camera:GetUp():mul(self.camSpeed):mul(dt))
    end
    if Input:KeyPressed(Inputs.KEY_F) then
        local pos = Input:CursorPos()
        if self.cursorPos.x >= 0 and self.cursorPos.y >= 0 then
            Camera:Rotate(__TS__New(Vector2, self.cursorPos.y - pos.y, self.cursorPos.x - pos.x):mul(180 / 600))
        end
    end
end
function CameraComponent.prototype.shutdown(self)
    print(nil, "camera component")
end
____exports.component = __TS__New(____exports.CameraComponent)
return ____exports
