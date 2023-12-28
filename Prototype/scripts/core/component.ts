import { Entity } from "./entity";
import { Guid } from "./guid";

export interface Component {
    initialize() : void;
    update(dt : number) : void;
    shutdown() : void
}

export class NativeComponent {

    public GetEntity() : Entity {
        return this.Entity;
    }

    private __native_set_id__(d1 : number, d2 : number) {
        this.Id = new Guid(d1, d2);
    }
    private __native_get_id__() {
        return $multi(this.Id.d1, this.Id.d2);
    }
    private __native_set_entity__(d1 : number, d2: number) {
        this.Entity = new Entity(d1, d2);
    }
    private __native_get_entity__() {
        return $multi(this.Entity.GetId().d1, this.Entity.GetId().d2);
    }
    private Id : Guid;
    private Entity : Entity;
}

