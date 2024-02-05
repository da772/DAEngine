local ____lualib = require("lualib_bundle")
local __TS__New = ____lualib.__TS__New
local ____exports = {}
local ____vector = require("daengine.vector")
local Vector3 = ____vector.Vector3
____exports.Camera = {}
local Camera = ____exports.Camera
do
    function Camera.Move(self, vector)
        native_camera_move(nil, vector.x, vector.y, vector.z)
    end
    function Camera.Rotate(self, vector)
        native_camera_rotate(nil, vector.x, vector.y)
    end
    function Camera.GetForward(self)
        local x, y, z = unpack(native_camera_get_forward(nil))
        return __TS__New(Vector3, x, y, z)
    end
    function Camera.GetRight(self)
        local x, y, z = unpack(native_camera_get_right(nil))
        return __TS__New(Vector3, x, y, z)
    end
    function Camera.GetUp(self)
        local x, y, z = unpack(native_camera_get_up(nil))
        return __TS__New(Vector3, x, y, z)
    end
    function Camera.GetPosition(self)
        local x, y, z = unpack(native_camera_get_position(nil))
        return __TS__New(Vector3, x, y, z)
    end
end
return ____exports
