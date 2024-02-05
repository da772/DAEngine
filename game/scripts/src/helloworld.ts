import { Employee } from "./lib";

import { ImGui } from "./daengine/imgui";
import { NativeComponent, Component } from "./daengine/component";
import { Debug } from "./daengine/debug";
import { Input } from "./daengine/input";
import { Vector3 } from "./daengine/vector";
import { Inputs } from "./daengine/input_enum";



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
            ImGui.LabelText(`employee is sick ${this.e.empName} is sick: ${this.e.empCode}`);
            ImGui.LabelText("Hello people");

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
