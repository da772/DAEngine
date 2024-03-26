local ____lualib = require("lualib_bundle")
local __TS__Class = ____lualib.__TS__Class
local __TS__New = ____lualib.__TS__New
local __TS__SourceMapTraceBack = ____lualib.__TS__SourceMapTraceBack
__TS__SourceMapTraceBack(debug.getinfo(1).short_src, {["7"] = 1,["8"] = 1,["9"] = 2,["10"] = 2,["11"] = 10,["12"] = 10,["13"] = 10,["15"] = 13,["16"] = 14,["17"] = 12,["18"] = 17,["19"] = 18,["20"] = 17});
local ____exports = {}
local ____entity = require("daengine.entity")
local Entity = ____entity.Entity
local ____guid = require("daengine.guid")
local Guid = ____guid.Guid
____exports.NativeComponent = __TS__Class()
local NativeComponent = ____exports.NativeComponent
NativeComponent.name = "NativeComponent"
function NativeComponent.prototype.____constructor(self)
    self.Id = __TS__New(Guid, "")
    self.Entity = __TS__New(Entity, "")
end
function NativeComponent.prototype.GetEntity(self)
    return self.Entity
end
return ____exports
