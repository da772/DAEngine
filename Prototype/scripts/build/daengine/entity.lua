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
function Entity.prototype.____constructor(self, d1, d2)
    self.Id = __TS__New(Guid, d1, d2)
end
function Entity.prototype.setPosition(self, position)
    native_entity_set_position(
        nil,
        self.Id.d1,
        self.Id.d2,
        position.x,
        position.y,
        position.z
    )
end
function Entity.prototype.getPosition(self)
    local result = native_entity_get_position(nil, self.Id.d1, self.Id.d2)
    return __TS__New(Vector3, result[1], result[2], result[3])
end
function Entity.prototype.GetId(self)
    return self.Id
end
return ____exports
