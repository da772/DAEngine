import { Employee } from "./lib";

import { ImGui } from "./core/imgui";
import { NativeComponent, Component } from "./core/component";
import { Debug } from "./core/debug";
import { Input } from "./core/input";
import { Vector3 } from "./core/vector";
import { Inputs } from "./core/input_enum";



export class MyComponent extends NativeComponent implements Component {

    e : Employee = new Employee(1, "Steve");

    initialize(): void {
        print("script init")
        Debug.RegisterDebugMenu("MyComponent", this, MyComponent.prototype.debugUpdate);
    }

    update(dt: number): void {
        if (Input.KeyPressed(Inputs.KEY_T)) // T
        {
            let pos = this.GetEntity().getPosition();
            this.GetEntity().setPosition(new Vector3(pos.x, pos.y, pos.z + 1.61)); 
        }
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
