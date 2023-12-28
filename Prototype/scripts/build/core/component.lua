local ____lualib = require("lualib_bundle")
local __TS__Class = ____lualib.__TS__Class
local __TS__New = ____lualib.__TS__New
local ____exports = {}
local ____entity = require("core.entity")
local Entity = ____entity.Entity
local ____guid = require("core.guid")
local Guid = ____guid.Guid
____exports.NativeComponent = __TS__Class()
local NativeComponent = ____exports.NativeComponent
NativeComponent.name = "NativeComponent"
function NativeComponent.prototype.____constructor(self)
end
function NativeComponent.prototype.GetEntity(self)
    return self.Entity
end
function NativeComponent.prototype.__native_set_id__(self, d1, d2)
    self.Id = __TS__New(Guid, d1, d2)
end
function NativeComponent.prototype.__native_get_id__(self)
    return self.Id.d1, self.Id.d2
end
function NativeComponent.prototype.__native_set_entity__(self, d1, d2)
    self.Entity = __TS__New(Entity, d1, d2)
end
function NativeComponent.prototype.__native_get_entity__(self)
    return self.Entity:GetId().d1, self.Entity:GetId().d2
end
return ____exports
