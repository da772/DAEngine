local ____lualib = require("lualib_bundle")
local __TS__Class = ____lualib.__TS__Class
local __TS__New = ____lualib.__TS__New
local ____exports = {}
____exports.Vector4 = __TS__Class()
local Vector4 = ____exports.Vector4
Vector4.name = "Vector4"
function Vector4.prototype.____constructor(self, x, y, z, w)
    if x == nil then
        x = 0
    end
    if y == nil then
        y = 0
    end
    if z == nil then
        z = 0
    end
    if w == nil then
        w = 0
    end
    self.x = x
    self.y = y
    self.z = z
    self.w = w
end
function Vector4.fromNative(self, v)
    return __TS__New(
        ____exports.Vector4,
        v.x,
        v.y,
        v.z,
        v.w
    )
end
function Vector4.prototype.equals(self, other)
    return other.x == self.x and other.y == self.y and other.z == self.z and other.w == self.w
end
function Vector4.prototype.neg(self)
    return __TS__New(
        ____exports.Vector4,
        -self.x,
        -self.y,
        -self.z,
        -self.w
    )
end
function Vector4.prototype.mul(self, v)
    if type(v) == "number" then
        return __TS__New(
            ____exports.Vector4,
            self.x * v,
            self.y * v,
            self.z * v,
            self.w * v
        )
    end
    return __TS__New(
        ____exports.Vector4,
        self.x * v.x,
        self.y * v.y,
        self.z * v.z,
        self.w * v.w
    )
end
____exports.Vector3 = __TS__Class()
local Vector3 = ____exports.Vector3
Vector3.name = "Vector3"
function Vector3.prototype.____constructor(self, x, y, z)
    if x == nil then
        x = 0
    end
    if y == nil then
        y = 0
    end
    if z == nil then
        z = 0
    end
    self.x = x
    self.y = y
    self.z = z
end
function Vector3.fromNative(self, v)
    return __TS__New(____exports.Vector3, v.x, v.y, v.z)
end
function Vector3.prototype.equals(self, other)
    return other.x == self.x and other.y == self.y and other.z == self.z
end
function Vector3.prototype.neg(self)
    return __TS__New(____exports.Vector3, -self.x, -self.y, -self.z)
end
function Vector3.prototype.mul(self, v)
    if type(v) == "number" then
        return __TS__New(____exports.Vector3, self.x * v, self.y * v, self.z * v)
    end
    return __TS__New(____exports.Vector3, self.x * v.x, self.y * v.y, self.z * v.z)
end
____exports.Vector2 = __TS__Class()
local Vector2 = ____exports.Vector2
Vector2.name = "Vector2"
function Vector2.prototype.____constructor(self, x, y)
    if x == nil then
        x = 0
    end
    if y == nil then
        y = 0
    end
    self.x = x
    self.y = y
end
function Vector2.fromNative(self, v)
    return __TS__New(____exports.Vector2, v.x, v.y)
end
function Vector2.prototype.equals(self, other)
    return other.x == self.x and other.y == self.y
end
function Vector2.prototype.neg(self)
    return __TS__New(____exports.Vector2, -self.x, -self.y)
end
function Vector2.prototype.mul(self, v)
    if type(v) == "number" then
        return __TS__New(____exports.Vector3, self.x * v, self.y * v)
    end
    return __TS__New(____exports.Vector3, self.x * v.x, self.y * v.y)
end
return ____exports
