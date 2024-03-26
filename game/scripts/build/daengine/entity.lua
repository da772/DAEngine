local ____lualib = require("lualib_bundle")
local __TS__Class = ____lualib.__TS__Class
local __TS__New = ____lualib.__TS__New
local __TS__SourceMapTraceBack = ____lualib.__TS__SourceMapTraceBack
__TS__SourceMapTraceBack(debug.getinfo(1).short_src, {["7"] = 1,["8"] = 1,["9"] = 2,["10"] = 2,["11"] = 4,["12"] = 4,["13"] = 4,["14"] = 5,["15"] = 6,["16"] = 5,["17"] = 9,["18"] = 10,["19"] = 9,["20"] = 13,["21"] = 14,["22"] = 13,["23"] = 17,["24"] = 18,["25"] = 17,["26"] = 21,["27"] = 22,["28"] = 21,["29"] = 25,["30"] = 26,["31"] = 25,["32"] = 29,["33"] = 30,["34"] = 30,["35"] = 30,["36"] = 30,["37"] = 30,["38"] = 30,["39"] = 30,["40"] = 29,["41"] = 33,["42"] = 34,["43"] = 33});
local ____exports = {}
local ____guid = require("daengine.guid")
local Guid = ____guid.Guid
local ____vector = require("daengine.vector")
local Vector3 = ____vector.Vector3
____exports.Entity = __TS__Class()
local Entity = ____exports.Entity
Entity.name = "Entity"
function Entity.prototype.____constructor(self, index)
    self.Id = __TS__New(Guid, index)
end
function Entity.prototype.setPosition(self, position)
    native_entity_set_position(nil, self.Id, position)
end
function Entity.prototype.getPosition(self)
    return Vector3:fromNative(native_entity_get_position(nil, self.Id))
end
function Entity.prototype.getForward(self)
    return Vector3:fromNative(native_entity_get_forward(nil, self.Id))
end
function Entity.prototype.getRight(self)
    return Vector3:fromNative(native_entity_get_right(nil, self.Id))
end
function Entity.prototype.getUp(self)
    return Vector3:fromNative(native_entity_get_up(nil, self.Id))
end
function Entity.prototype.applyVelocity(self, vel)
    native_entity_apply_velocity(
        nil,
        self.Id.Index,
        vel.x,
        vel.y,
        vel.z
    )
end
function Entity.prototype.GetId(self)
    return self.Id
end
return ____exports
