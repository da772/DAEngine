local ____lualib = require("lualib_bundle")
local __TS__SourceMapTraceBack = ____lualib.__TS__SourceMapTraceBack
__TS__SourceMapTraceBack(debug.getinfo(1).short_src, {["5"] = 2,["6"] = 2,["8"] = 4,["10"] = 4,["11"] = 6,["12"] = 5,["13"] = 4,["14"] = 10,["15"] = 9,["16"] = 4,["17"] = 14,["18"] = 13});
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
    function Input.MousePressed(self, key)
        return native_input_mouse_pressed(nil, key)
    end
end
return ____exports
