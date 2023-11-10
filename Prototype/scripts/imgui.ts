export namespace ImGui {
    export function Begin(title : string) : boolean {
        return native_imgui_begin(title);
    } 
    export function Button(title : string) : boolean {
        return native_imgui_button(title);
    }
    export function End() : void {
        native_imgui_end();
    };
    export function LabelText(title : string, text : string) : void {
        native_imgui_label_text(title, text);
    };
}

declare function native_imgui_begin(title : string) : boolean;
declare function native_imgui_button(title : string) : boolean;
declare function native_imgui_label_text(title : string, text : string) : boolean;
declare function native_imgui_end() : void;