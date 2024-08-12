local ____lualib = require("lualib_bundle")
local __TS__SourceMapTraceBack = ____lualib.__TS__SourceMapTraceBack
__TS__SourceMapTraceBack(debug.getinfo(1).short_src, {["6"] = 1,["8"] = 1,["9"] = 3,["10"] = 3,["11"] = 3,["12"] = 3,["13"] = 3,["14"] = 3,["15"] = 3,["16"] = 2,["17"] = 1,["18"] = 6,["19"] = 5});
local ____exports = {}
____exports.Debug = {}
local Debug = ____exports.Debug
do
    function Debug.RegisterDebugMenu(self, id, object, func)
        native_debug_register_debug_menu(
            nil,
            "Script",
            id,
            object,
            func
        )
    end
    function Debug.UnregisterDebugMenu(self, id)
        native_debug_unregister_debug_menu(nil, "Script", id)
    end
end
return ____exports
