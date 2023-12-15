import { Component } from "./core/component";
import { Input } from "./core/input";
import { Camera } from "./core/camera";
import { Vector2 } from "./core/vector";

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
        if (Input.KeyPressed(87)) // W
        {
            Camera.Move(Camera.GetForward().mul(this.camSpeed).mul(dt));
        }

        if (Input.KeyPressed(83)) // S
        {
            Camera.Move(Camera.GetForward().neg().mul(this.camSpeed).mul(dt));
        }

        if (Input.KeyPressed(65)) // A
        {
            Camera.Move(Camera.GetRight().neg().mul(this.camSpeed).mul(dt));
        }

        if (Input.KeyPressed(68)) // D
        {
            Camera.Move(Camera.GetRight().mul(this.camSpeed).mul(dt));
        }

        if (Input.KeyPressed(32)) // Space
        {
            Camera.Move(Camera.GetUp().mul(this.camSpeed).mul(dt));
        }

        if (Input.KeyPressed(341)) // LCtrl
        {
            Camera.Move(Camera.GetUp().neg().mul(this.camSpeed).mul(dt));
        }

        if (Input.KeyPressed(70)) // F
        {
            let pos : Vector2 = Input.CursorPos();
            if (this.cursorPos.x >= 0.0 && this.cursorPos.y >= 0.0)
            {
                Camera.Rotate(new Vector2(pos.y - this.cursorPos.y, pos.x - this.cursorPos.x).neg().mul(180.0 / 600.0));
            }
        }
    }
    
    shutdown(): void {
        print("camera component");
    }
    
}

export let component : CameraComponent = new CameraComponent();
