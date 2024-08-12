local ____lualib = require("lualib_bundle")
local __TS__SourceMapTraceBack = ____lualib.__TS__SourceMapTraceBack
__TS__SourceMapTraceBack(debug.getinfo(1).short_src, {["5"] = 1,["6"] = 1,["8"] = 3,["10"] = 3,["11"] = 5,["12"] = 4,["13"] = 3,["14"] = 9,["15"] = 8,["16"] = 3,["17"] = 13,["18"] = 12,["19"] = 3,["20"] = 17,["21"] = 16,["22"] = 3,["23"] = 21,["24"] = 20,["25"] = 3,["26"] = 25,["27"] = 24});
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
