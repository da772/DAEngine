export interface Component {
    initialize() : void;
    update(dt : number) : void;
    shutdown() : void
}