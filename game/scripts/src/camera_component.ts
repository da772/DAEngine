
import { Camera } from "./daengine/camera";
import { Component, NativeComponent } from "./daengine/component";
import { Debug } from "./daengine/debug";
import { ImGui } from "./daengine/imgui";
import { Input } from "./daengine/input";
import { Inputs } from "./daengine/input_enum";
import { Timer } from "./daengine/timer";
import { Vector2 } from "./daengine/vector";


export class CameraComponent extends NativeComponent implements Component {

    cursorPos : Vector2 = new Vector2();
    camSpeed : number = 5;
    velSpeed : number = 5000;

    testFuncAsync = async () => {
        print("waiting for timer")
        await Timer.runAsync(5000);
        print("Timer compelte!");
    }

    initialize(): void {
        print("camera component init");
        Debug.RegisterDebugMenu("CameraComponent", this, CameraComponent.prototype.debugUpdate);
        this.testFuncAsync();
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
        if (Input.KeyPressed(Inputs.KEY_UP)) // W
        {
            Camera.Move(Camera.GetForward().mul(this.camSpeed).mul(dt));
        }

        if (Input.KeyPressed(Inputs.KEY_DOWN)) // S
        {
            Camera.Move(Camera.GetForward().neg().mul(this.camSpeed).mul(dt));
        }

        if (Input.KeyPressed(Inputs.KEY_LEFT)) // A
        {
            Camera.Move(Camera.GetRight().neg().mul(this.camSpeed).mul(dt));
        }

        if (Input.KeyPressed(Inputs.KEY_RIGHT)) // D
        {
            Camera.Move(Camera.GetRight().mul(this.camSpeed).mul(dt));
        }
        
        if (Input.KeyPressed(Inputs.KEY_E)) // E
        {
            Camera.Move(Camera.GetUp().neg().mul(this.camSpeed).mul(dt));
        }

        if (Input.KeyPressed(Inputs.KEY_Q)) // Q
        {
            Camera.Move(Camera.GetUp().mul(this.camSpeed).mul(dt));
        }

        if (Input.MousePressed(Inputs.MOUSE_BUTTON_2)) // F
        {
            let pos : Vector2 = Input.CursorPos();
            if (this.cursorPos.x >= 0.0 && this.cursorPos.y >= 0.0)
            {
             //   Camera.Rotate(new Vector2(this.cursorPos.y - pos.y, this.cursorPos.x - pos.x).mul(180.0 / 600.0));
            }
        }
    }

    debugUpdate()
    {
        if (ImGui.Begin("Camera Component")) {
            ImGui.LabelText("Camera Speed");
            ImGui.SameLine();
            this.camSpeed = ImGui.InputFloat("##CamSpeed", this.camSpeed);
        }
        
        ImGui.End();
    }


    shutdown(): void {
        print("camera component shutdown");
        Debug.UnregisterDebugMenu("CameraComponent");
    }
    
}

export let component : CameraComponent = new CameraComponent();