local ____lualib = require("lualib_bundle")
local __TS__SourceMapTraceBack = ____lualib.__TS__SourceMapTraceBack
__TS__SourceMapTraceBack(debug.getinfo(1).short_src, {["5"] = 1,["6"] = 1,["8"] = 3,["10"] = 3,["11"] = 5,["12"] = 4,["13"] = 3,["14"] = 9,["15"] = 8});
local ____exports = {}
local ____vector = require("daengine.vector")
local Vector2 = ____vector.Vector2
____exports.Input = {}
local Input = ____exports.Input
do
    function Input.KeyPressed(self, key)
        return native_input_key_pressed(nil, key)
    end
    function Input.CursorPos(self)
        return Vector2:fromNative(native_input_cursor_pos(nil))
    end
end
return ____exports
