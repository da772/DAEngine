local ____lualib = require("lualib_bundle")
local __TS__Class = ____lualib.__TS__Class
local __TS__SourceMapTraceBack = ____lualib.__TS__SourceMapTraceBack
__TS__SourceMapTraceBack(debug.getinfo(1).short_src, {["6"] = 1,["7"] = 1,["8"] = 1,["9"] = 5,["10"] = 6,["11"] = 7,["12"] = 5,["13"] = 10,["14"] = 11,["15"] = 10});
local ____exports = {}
____exports.Employee = __TS__Class()
local Employee = ____exports.Employee
Employee.name = "Employee"
function Employee.prototype.____constructor(self, code, name)
    self.empName = name
    self.empCode = code
end
function Employee.prototype.getSalary(self)
    return 10000
end
return ____exports
