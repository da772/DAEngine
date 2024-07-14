local ____lualib = require("lualib_bundle")
local __TS__Class = ____lualib.__TS__Class
local __TS__New = ____lualib.__TS__New
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
