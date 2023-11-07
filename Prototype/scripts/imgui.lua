--[[ Generated with https://github.com/TypeScriptToLua/TypeScriptToLua ]]
local ____exports = {}
____exports.ImGui = {}
local ImGui = ____exports.ImGui
do
    function ImGui.Begin(self, title)
        return native_imgui_begin(nil, title)
    end
    function ImGui.End(self)
        native_imgui_end(nil)
    end
    function ImGui.LabelText(self, title, text)
        native_imgui_label_text(nil, title, text)
    end
end
return ____exports
