import { Vector2, Vector3 } from "./vector";

export namespace Camera {
    export function Move(vector : Vector3) : void {
        native_camera_move(vector);
    } 

    export function Rotate(vector : Vector2) : void {
        native_camera_rotate(vector);
    } 

    export function GetForward() : Vector3 {
        return Vector3.fromNative(native_camera_get_forward());
    } 

    export function GetRight() : Vector3 {
        return Vector3.fromNative(native_camera_get_right());
    }

    export function GetUp() : Vector3 {
        return Vector3.fromNative(native_camera_get_up());
    }

    export function GetPosition() : Vector3 {
        return Vector3.fromNative(native_camera_get_position());
    }
}

declare function native_camera_move(pos : Vector3) : void;
declare function native_camera_rotate(rot : Vector2) : void;
declare function native_camera_get_forward() : Vector3
declare function native_camera_get_right() : Vector3;
declare function native_camera_get_up() : Vector3;
declare function native_camera_get_position() : Vector3;