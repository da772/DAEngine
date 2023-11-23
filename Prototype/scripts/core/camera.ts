import { Vector3, Vector2 } from "./vector";

export namespace Camera {
    export function Move(vector : Vector3) : void {
        native_camera_move(vector.x, vector.y, vector.z);
    } 

    export function Rotate(vector : Vector2) : void {
        native_camera_rotate(vector.x, vector.y);
    } 

    export function GetForward() : Vector3 {
        let [x, y, z] = native_camera_get_forward();
        return new Vector3(x,y,z);
    } 

    export function GetRight() : Vector3 {
        let [x, y, z] = native_camera_get_right();
        return new Vector3(x,y,z);
    }

    export function GetUp() : Vector3 {
        let [x, y, z] = native_camera_get_up();
        return new Vector3(x,y,z);
    }

    export function GetPosition() : Vector3 {
        let [x, y, z] = native_camera_get_position();
        return new Vector3(x,y,z);
    }
}

declare function native_camera_move(x : number,y : number,z : number) : void;
declare function native_camera_rotate(x : number,y : number) : void;
declare function native_camera_get_forward() : [number, number, number];
declare function native_camera_get_right() : [number, number, number];
declare function native_camera_get_up() : [number, number, number];
declare function native_camera_get_position() : [number, number, number];