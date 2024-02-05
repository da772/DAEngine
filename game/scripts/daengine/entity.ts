import { Guid } from "./guid";
import { Vector3 } from "./vector";

export class Entity {
    constructor(d1 : number, d2: number) {
        this.Id = new Guid(d1, d2);
    }

    setPosition(position : Vector3) {
        native_entity_set_position(this.Id.d1, this.Id.d2, position.x, position.y, position.z);
    }

    getPosition() : Vector3 {
        let result = native_entity_get_position(this.Id.d1, this.Id.d2);
        return new Vector3(result[0], result[1], result[2]);
    }

    GetId() : Guid {
        return this.Id;
    }

    private Id : Guid;
}

declare function native_entity_set_position(id1 : number, id2 : number, x : number,y : number,z : number) : void;
declare function native_entity_get_position(id1 : number, id2: number) : [number, number, number];
