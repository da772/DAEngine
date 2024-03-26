local ____lualib = require("lualib_bundle")
local __TS__SourceMapTraceBack = ____lualib.__TS__SourceMapTraceBack
__TS__SourceMapTraceBack(debug.getinfo(1).short_src, {["6"] = 1,["8"] = 1,["9"] = 3,["10"] = 2,["11"] = 1,["12"] = 6,["13"] = 5,["14"] = 1,["15"] = 9,["16"] = 8,["17"] = 1,["18"] = 11,["19"] = 11,["21"] = 12,["22"] = 11,["23"] = 1,["24"] = 15,["25"] = 14,["26"] = 1,["27"] = 18,["28"] = 17});
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
