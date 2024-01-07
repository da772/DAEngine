local ____lualib = require("lualib_bundle")
local __TS__New = ____lualib.__TS__New
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
        local x, y = unpack(native_input_cursor_pos(nil))
        return __TS__New(Vector2, x, y)
    end
end
return ____exports
