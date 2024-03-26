import { Entity } from "./entity";
import { Guid } from "./guid";

export interface Component {
    initialize() : void;
    update(dt : number) : void;
    shutdown() : void
}

export class NativeComponent {

    constructor() {
        this.Id = new Guid("");
        this.Entity = new Entity("");
    }

    public GetEntity() : Entity {
        return this.Entity;
    }

    protected Id : Guid;
    protected Entity : Entity;
}

