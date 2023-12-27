import { Employee } from "./lib";

import { ImGui } from "./core/imgui";
import { Component } from "./core/component";
import { Debug } from "./core/debug";



export class MyComponent implements Component {

    e : Employee = new Employee(1, "Steve");

    initialize(): void {
        print("script init")
        Debug.RegisterDebugMenu("MyComponent", this, MyComponent.prototype.debugUpdate);
    }
    update(dt: number): void {
       

    }

    debugUpdate(this : MyComponent)
    {
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
        Debug.UnregisterDebugMenu("MyComponent");
    }
    
}

export let component : MyComponent = new MyComponent();
