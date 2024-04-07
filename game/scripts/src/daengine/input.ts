import { Inputs } from "./input_enum";
import { Vector2 } from "./vector";

export namespace Input {
    export function KeyPressed(key : Inputs) : boolean {
        return native_input_key_pressed(key);
    } 

    export function CursorPos() : Vector2 {
        return Vector2.fromNative(native_input_cursor_pos());
    } 

    export function MousePressed(key : Inputs) : boolean {
        return native_input_mouse_pressed(key);
    }
}

declare function native_input_key_pressed(key : number) : boolean;
declare function native_input_cursor_pos() : Vector2;
declare function native_input_mouse_pressed(key : number) : boolean;