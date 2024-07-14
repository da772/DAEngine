local ____lualib = require("lualib_bundle")
local __TS__Class = ____lualib.__TS__Class
local __TS__New = ____lualib.__TS__New
local ____exports = {}
____exports.Guid = __TS__Class()
local Guid = ____exports.Guid
Guid.name = "Guid"
function Guid.prototype.____constructor(self, index)
    self.Index = index
end
____exports.guid = __TS__New(____exports.Guid, "")
return ____exports
