local ____lualib = require("lualib_bundle")
local __TS__Class = ____lualib.__TS__Class
local __TS__New = ____lualib.__TS__New
local __TS__SourceMapTraceBack = ____lualib.__TS__SourceMapTraceBack
__TS__SourceMapTraceBack(debug.getinfo(1).short_src, {["7"] = 1,["8"] = 1,["9"] = 1,["10"] = 8,["11"] = 8,["12"] = 8,["14"] = 8,["15"] = 8,["17"] = 8,["18"] = 8,["20"] = 8,["21"] = 8,["23"] = 9,["24"] = 10,["25"] = 11,["26"] = 12,["27"] = 8,["28"] = 15,["29"] = 16,["30"] = 16,["31"] = 16,["32"] = 16,["33"] = 16,["34"] = 16,["35"] = 16,["36"] = 15,["37"] = 19,["38"] = 20,["39"] = 19,["40"] = 23,["41"] = 25,["42"] = 25,["43"] = 25,["44"] = 25,["45"] = 25,["46"] = 25,["47"] = 25,["48"] = 23,["49"] = 31,["50"] = 33,["51"] = 34,["52"] = 34,["53"] = 34,["54"] = 34,["55"] = 34,["56"] = 34,["57"] = 34,["59"] = 36,["60"] = 36,["61"] = 36,["62"] = 36,["63"] = 36,["64"] = 36,["65"] = 36,["66"] = 31,["67"] = 40,["68"] = 40,["69"] = 40,["70"] = 46,["71"] = 46,["72"] = 46,["74"] = 46,["75"] = 46,["77"] = 46,["78"] = 46,["80"] = 47,["81"] = 48,["82"] = 49,["83"] = 46,["84"] = 52,["85"] = 53,["86"] = 52,["87"] = 56,["88"] = 57,["89"] = 56,["90"] = 60,["91"] = 62,["92"] = 60,["93"] = 68,["94"] = 70,["95"] = 71,["97"] = 73,["98"] = 68,["99"] = 78,["100"] = 78,["101"] = 78,["102"] = 83,["103"] = 83,["104"] = 83,["106"] = 83,["107"] = 83,["109"] = 84,["110"] = 85,["111"] = 83,["112"] = 88,["113"] = 89,["114"] = 88,["115"] = 92,["116"] = 93,["117"] = 92,["118"] = 96,["119"] = 98,["120"] = 96,["121"] = 104,["122"] = 106,["123"] = 107,["125"] = 109,["126"] = 104});
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
