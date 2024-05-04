local ____lualib = require("lualib_bundle")
local __TS__Class = ____lualib.__TS__Class
local __TS__ClassExtends = ____lualib.__TS__ClassExtends
local __TS__New = ____lualib.__TS__New
local __TS__AsyncAwaiter = ____lualib.__TS__AsyncAwaiter
local __TS__Await = ____lualib.__TS__Await
local __TS__SourceMapTraceBack = ____lualib.__TS__SourceMapTraceBack
__TS__SourceMapTraceBack(debug.getinfo(1).short_src, {["10"] = 2,["11"] = 2,["12"] = 3,["13"] = 3,["14"] = 4,["15"] = 4,["16"] = 5,["17"] = 5,["18"] = 6,["19"] = 6,["20"] = 7,["21"] = 7,["22"] = 8,["23"] = 8,["24"] = 9,["25"] = 9,["26"] = 12,["27"] = 12,["28"] = 12,["29"] = 12,["31"] = 12,["32"] = 14,["33"] = 15,["34"] = 16,["35"] = 18,["37"] = 19,["38"] = 20,["39"] = 21,["41"] = 18,["42"] = 12,["43"] = 24,["44"] = 25,["45"] = 26,["46"] = 27,["47"] = 24,["48"] = 29,["49"] = 30,["50"] = 32,["51"] = 34,["52"] = 36,["54"] = 29,["55"] = 41,["56"] = 42,["57"] = 44,["59"] = 47,["60"] = 49,["62"] = 52,["63"] = 54,["65"] = 57,["66"] = 59,["68"] = 62,["69"] = 64,["71"] = 67,["72"] = 69,["74"] = 72,["75"] = 74,["76"] = 75,["77"] = 77,["80"] = 41,["81"] = 82,["82"] = 84,["83"] = 85,["84"] = 86,["85"] = 87,["87"] = 90,["88"] = 82,["89"] = 94,["90"] = 95,["91"] = 96,["92"] = 94,["93"] = 101});
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
