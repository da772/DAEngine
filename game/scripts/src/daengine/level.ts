export interface Level {
    initialize() : void;
    update(dt : number) : void;
    shutdown() : void
}