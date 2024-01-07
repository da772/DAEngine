import { Component } from "./daengine/component";
import { Input } from "./daengine/input";
import { Camera } from "./daengine/camera";
import { Vector2 } from "./daengine/vector";
import { Inputs } from "./daengine/input_enum";

export class CameraComponent implements Component {

    cursorPos : Vector2 = new Vector2();
    camSpeed : number = .1;

    initialize(): void {
        print("camera component init");
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

        if (Input.KeyPressed(Inputs.KEY_LEFT_CONTROL)) // LCtrl
        {
            Camera.Move(Camera.GetUp().neg().mul(this.camSpeed).mul(dt));
        }

        if (Input.KeyPressed(Inputs.KEY_LEFT_SHIFT)) // LShfit
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
    
    shutdown(): void {
        print("camera component");
    }
    
}

export let component : CameraComponent = new CameraComponent();
