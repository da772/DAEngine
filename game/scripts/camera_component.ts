import { Component } from "./daengine/component";
import { Input } from "./daengine/input";
import { Camera } from "./daengine/camera";
import { Vector2 } from "./daengine/vector";
import { Inputs } from "./daengine/input_enum";
import { Debug } from "./daengine/debug";
import { ImGui } from "./daengine/imgui";

export class CameraComponent implements Component {

    cursorPos : Vector2 = new Vector2();
    camSpeed : number = 10;

    initialize(): void {
        print("camera component init");
        Debug.RegisterDebugMenu("CameraComponent", this, CameraComponent.prototype.debugUpdate);
    }
    update(dt: number): void {
        this.cameraInput(dt);
        
        let pos : Vector2 = Input.CursorPos();
        
        if (!pos.equals(this.cursorPos))
        {
            this.cursorPos = pos;
        }
    }


    cameraInput(dt : number) : void {
        if (Input.KeyPressed(Inputs.KEY_W)) // W
        {
            Camera.Move(Camera.GetForward().mul(this.camSpeed).mul(dt));
        }

        if (Input.KeyPressed(Inputs.KEY_S)) // S
        {
            Camera.Move(Camera.GetForward().neg().mul(this.camSpeed).mul(dt));
        }

        if (Input.KeyPressed(Inputs.KEY_A)) // A
        {
            Camera.Move(Camera.GetRight().neg().mul(this.camSpeed).mul(dt));
        }

        if (Input.KeyPressed(Inputs.KEY_D)) // D
        {
            Camera.Move(Camera.GetRight().mul(this.camSpeed).mul(dt));
        }

        if (Input.KeyPressed(Inputs.KEY_SPACE)) // Space
        {
            Camera.Move(Camera.GetUp().mul(this.camSpeed).mul(dt));
        }

        if (Input.KeyPressed(Inputs.KEY_E)) // E
        {
            Camera.Move(Camera.GetUp().neg().mul(this.camSpeed).mul(dt));
        }

        if (Input.KeyPressed(Inputs.KEY_Q)) // Q
        {
            Camera.Move(Camera.GetUp().mul(this.camSpeed).mul(dt));
        }

        if (Input.KeyPressed(Inputs.KEY_F)) // F
        {
            let pos : Vector2 = Input.CursorPos();
            if (this.cursorPos.x >= 0.0 && this.cursorPos.y >= 0.0)
            {
                Camera.Rotate(new Vector2(this.cursorPos.y - pos.y, this.cursorPos.x - pos.x).mul(180.0 / 600.0));
            }
        }
    }
    
    debugUpdate(this : CameraComponent)
    {
        if (ImGui.Begin("Camera Component")) {
            ImGui.LabelText("Camera Speed");
            ImGui.SameLine();
            this.camSpeed = ImGui.InputFloat("##CamSpeed", this.camSpeed);
        }
        
        ImGui.End();
    }


    shutdown(): void {
        print("camera component");
        Debug.UnregisterDebugMenu("CameraComponent");
    }
    
}

export let component : CameraComponent = new CameraComponent();
