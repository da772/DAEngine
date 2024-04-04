local ____lualib = require("lualib_bundle")
local __TS__Class = ____lualib.__TS__Class
local __TS__Promise = ____lualib.__TS__Promise
local __TS__New = ____lualib.__TS__New
local __TS__SourceMapTraceBack = ____lualib.__TS__SourceMapTraceBack
__TS__SourceMapTraceBack(debug.getinfo(1).short_src, {["8"] = 1,["9"] = 1,["10"] = 1,["12"] = 8,["13"] = 8,["14"] = 8,["15"] = 9,["16"] = 10,["17"] = 8,["18"] = 8,["19"] = 7,["20"] = 14,["21"] = 15,["22"] = 14,["23"] = 18,["24"] = 19,["25"] = 18,["26"] = 22,["27"] = 23,["28"] = 22,["29"] = 27});
local ____exports = {}
____exports.PromiseCompletionSource = __TS__Class()
local PromiseCompletionSource = ____exports.PromiseCompletionSource
PromiseCompletionSource.name = "PromiseCompletionSource"
function PromiseCompletionSource.prototype.____constructor(self)
    self._promise = __TS__New(
        __TS__Promise,
        function(____, resolve, reject)
            self._resolve = resolve
            self._reject = reject
        end
    )
end
function PromiseCompletionSource.prototype.Promise(self)
    return self._promise
end
function PromiseCompletionSource.prototype.Resolve(self, val)
    self:_resolve(val)
end
function PromiseCompletionSource.prototype.Reject(self)
    self:_reject()
end
____exports.promise = __TS__New(____exports.PromiseCompletionSource)
return ____exports
