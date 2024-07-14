local ____lualib = require("lualib_bundle")
local __TS__Class = ____lualib.__TS__Class
local __TS__ClassExtends = ____lualib.__TS__ClassExtends
local __TS__New = ____lualib.__TS__New
local __TS__AsyncAwaiter = ____lualib.__TS__AsyncAwaiter
local __TS__Await = ____lualib.__TS__Await
local ____exports = {}
local ____camera = require("daengine.camera")
local Camera = ____camera.Camera
local ____component = require("daengine.component")
local NativeComponent = ____component.NativeComponent
local ____debug = require("daengine.debug")
local Debug = ____debug.Debug
local ____imgui = require("daengine.imgui")
local ImGui = ____imgui.ImGui
local ____input = require("daengine.input")
local Input = ____input.Input
local ____input_enum = require("daengine.input_enum")
local Inputs = ____input_enum.Inputs
local ____timer = require("daengine.timer")
local Timer = ____timer.Timer
local ____vector = require("daengine.vector")
local Vector2 = ____vector.Vector2
____exports.CameraComponent = __TS__Class()
local CameraComponent = ____exports.CameraComponent
CameraComponent.name = "CameraComponent"
__TS__ClassExtends(CameraComponent, NativeComponent)
function CameraComponent.prototype.____constructor(self, ...)
    NativeComponent.prototype.____constructor(self, ...)
    self.cursorPos = __TS__New(Vector2)
    self.camSpeed = 5
    self.velSpeed = 5000
    self.testFuncAsync = function()
        return __TS__AsyncAwaiter(function(____awaiter_resolve)
            print("waiting for timer")
            __TS__Await(Timer:runAsync(5000))
            print("Timer compelte!")
        end)
    end
end
function CameraComponent.prototype.initialize(self)
    print("camera component init")
    Debug:RegisterDebugMenu("CameraComponent", self, ____exports.CameraComponent.prototype.debugUpdate)
    self:testFuncAsync()
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
