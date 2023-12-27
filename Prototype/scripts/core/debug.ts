export namespace Debug {
    export function RegisterDebugMenu(id : string, object : any, func : () => void) : void {
        native_debug_register_debug_menu(id, object, func);
    } 
    export function UnregisterDebugMenu(id : string) : void {
        native_debug_unregister_debug_menu(id);
    }
}

declare function native_debug_register_debug_menu(id : string, object: any, func : () => void) : void;
declare function native_debug_unregister_debug_menu(id : string) : void;