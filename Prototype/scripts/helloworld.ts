import { Employee } from "./lib";

import { ImGui } from "./imgui";

let e : Employee = new Employee(1, "Steve");

//print("Your mom" + e.empCode + " " + e.empName + "swag");


if (ImGui.Begin("Hello World!")) {
    ImGui.LabelText("employee is sick", `${e.empName} is sick`);
}

ImGui.End();