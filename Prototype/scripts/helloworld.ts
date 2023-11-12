import { Employee } from "./lib";

import { ImGui } from "./core/imgui";
import { Component } from "./core/component";

export class MyComponent implements Component {

    e : Employee = new Employee(1, "Steve");

    initialize(): void {
        print("script init")
    }
    update(dt: number): void {
        if (ImGui.Begin("Hello World!")) {
            ImGui.LabelText("employee is sick", `${this.e.empName} is sick: ${this.e.empCode}`);
            ImGui.LabelText("###end", "Hello people");

            if (ImGui.Button("Press Me")) {
                this.e.empCode++;
            }

        }
        
        ImGui.End();
    }
    shutdown(): void {
        print("shut down");
    }
    
}

export let component : MyComponent = new MyComponent();
