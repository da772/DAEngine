local ____lualib = require("lualib_bundle")
local __TS__Class = ____lualib.__TS__Class
local __TS__New = ____lualib.__TS__New
local __TS__SourceMapTraceBack = ____lualib.__TS__SourceMapTraceBack
__TS__SourceMapTraceBack(debug.getinfo(1).short_src, {["7"] = 1,["8"] = 1,["9"] = 1,["10"] = 3,["11"] = 4,["12"] = 3,["13"] = 10});
local ____exports = {}
____exports.Guid = __TS__Class()
local Guid = ____exports.Guid
Guid.name = "Guid"
function Guid.prototype.____constructor(self, index)
    self.Index = index
end
____exports.guid = __TS__New(____exports.Guid, "")
return ____exports
