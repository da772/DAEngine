--[[ Generated with https://github.com/TypeScriptToLua/TypeScriptToLua ]]
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
