local ____lualib = require("lualib_bundle")
local __TS__New = ____lualib.__TS__New
local ____exports = {}
local ____lib = require("lib")
local Employee = ____lib.Employee
local ____imgui = require("imgui")
local ImGui = ____imgui.ImGui
local e = __TS__New(Employee, 1, "Steve")
if ImGui:Begin("Hello World!") then
    ImGui:LabelText("employee is sick", e.empName .. " is sick")
end
ImGui:End()
return ____exports
