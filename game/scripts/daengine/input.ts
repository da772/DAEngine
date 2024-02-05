import { Vector2 } from "./vector";

export namespace Input {
    export function KeyPressed(key : number) : boolean {
        return native_input_key_pressed(key);
    } 

    export function CursorPos() : Vector2 {
        let [x,y] = native_input_cursor_pos();
        return new Vector2(x,y);
    } 
}

declare function native_input_key_pressed(key : number) : boolean;
declare function native_input_cursor_pos() : [number, number];