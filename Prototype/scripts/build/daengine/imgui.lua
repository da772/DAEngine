--[[ Generated with https://github.com/TypeScriptToLua/TypeScriptToLua ]]
local ____exports = {}
____exports.ImGui = {}
local ImGui = ____exports.ImGui
do
    function ImGui.Begin(self, title)
        return native_imgui_begin(nil, title)
    end
    function ImGui.Button(self, title)
        return native_imgui_button(nil, title)
    end
    function ImGui.End(self)
        native_imgui_end(nil)
    end
    function ImGui.LabelText(self, title, text)
        if text == nil then
            text = ""
        end
        native_imgui_label_text(nil, title, text)
    end
    function ImGui.InputFloat(self, title, value)
        return native_imgui_float_input(nil, title, value)
    end
    function ImGui.SameLine(self)
        native_imgui_same_line(nil)
    end
end
return ____exports
