local ____lualib = require("lualib_bundle")
local __TS__Class = ____lualib.__TS__Class
local __TS__New = ____lualib.__TS__New
local __TS__SourceMapTraceBack = ____lualib.__TS__SourceMapTraceBack
__TS__SourceMapTraceBack(debug.getinfo(1).short_src, {["7"] = 1,["8"] = 1,["9"] = 1,["10"] = 7,["11"] = 7,["12"] = 7,["14"] = 7,["15"] = 7,["17"] = 7,["18"] = 7,["20"] = 8,["21"] = 9,["22"] = 10,["23"] = 7,["24"] = 13,["25"] = 14,["26"] = 13,["27"] = 17,["28"] = 18,["29"] = 17,["30"] = 21,["31"] = 23,["32"] = 21,["33"] = 29,["34"] = 31,["35"] = 32,["37"] = 34,["38"] = 29,["39"] = 39,["40"] = 39,["41"] = 39,["42"] = 44,["43"] = 44,["44"] = 44,["46"] = 44,["47"] = 44,["49"] = 45,["50"] = 46,["51"] = 44,["52"] = 49,["53"] = 50,["54"] = 49,["55"] = 53,["56"] = 54,["57"] = 53,["58"] = 57,["59"] = 59,["60"] = 57,["61"] = 65,["62"] = 67,["63"] = 68,["65"] = 70,["66"] = 65});
local ____exports = {}
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
