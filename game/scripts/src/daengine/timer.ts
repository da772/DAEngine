import { PromiseCompletionSource } from "./promise";

export class Timer {

    static runAsync(ms : number) : Promise<boolean> {
        return native_timer_create(ms);
    }
}

declare function native_timer_create(ms : number) : Promise<boolean>;