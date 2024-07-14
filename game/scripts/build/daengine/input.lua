--[[ Generated with https://github.com/TypeScriptToLua/TypeScriptToLua ]]
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
