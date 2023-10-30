local ____lualib = require("lualib_bundle")
local __TS__Class = ____lualib.__TS__Class
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
