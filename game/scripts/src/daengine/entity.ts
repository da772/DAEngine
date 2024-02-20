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

    getForward() : Vector3 {
        let result = native_entity_get_forward(this.Id.d1, this.Id.d2);
        return new Vector3(result[0], result[1], result[2]);
    }

    getRight() : Vector3 {
        let result = native_entity_get_right(this.Id.d1, this.Id.d2);
        return new Vector3(result[0], result[1], result[2]);
    }

    getUp() : Vector3 {
        let result = native_entity_get_up(this.Id.d1, this.Id.d2);
        return new Vector3(result[0], result[1], result[2]);
    }

    applyVelocity(vel : Vector3) : void {
        native_entity_apply_velocity(this.Id.d1, this.Id.d2, vel.x, vel.y, vel.z);
    }

    GetId() : Guid {
        return this.Id;
    }

    private Id : Guid;
}

declare function native_entity_set_position(id1 : number, id2 : number, x : number,y : number,z : number) : void;
declare function native_entity_get_position(id1 : number, id2: number) : [number, number, number];
declare function native_entity_get_forward(id1 : number, id2: number) : [number, number, number];
declare function native_entity_get_up(id1 : number, id2: number) : [number, number, number];
declare function native_entity_get_right(id1 : number, id2: number) : [number, number, number];

declare function native_entity_apply_velocity(id1 : number, id2 : number, x : number,y : number,z : number) : void;