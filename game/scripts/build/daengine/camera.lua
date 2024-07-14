--[[ Generated with https://github.com/TypeScriptToLua/TypeScriptToLua ]]
local ____exports = {}
local ____vector = require("daengine.vector")
local Vector3 = ____vector.Vector3
____exports.Camera = {}
local Camera = ____exports.Camera
do
    function Camera.Move(self, vector)
        native_camera_move(nil, vector)
    end
    function Camera.Rotate(self, vector)
        native_camera_rotate(nil, vector)
    end
    function Camera.GetForward(self)
        return Vector3:fromNative(native_camera_get_forward(nil))
    end
    function Camera.GetRight(self)
        return Vector3:fromNative(native_camera_get_right(nil))
    end
    function Camera.GetUp(self)
        return Vector3:fromNative(native_camera_get_up(nil))
    end
    function Camera.GetPosition(self)
        return Vector3:fromNative(native_camera_get_position(nil))
    end
end
return ____exports
