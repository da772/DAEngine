declare function print(s : any);

export interface Component {
    initialize() : void;
    update(dt : number) : void;
    shutdown() : void
}