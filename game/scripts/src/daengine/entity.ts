import { Guid } from "./guid";
import { Vector3 } from "./vector";

export class Entity {
    constructor(index : string) {
        this.Id = new Guid(index);
    }

    setPosition(position : Vector3) {
        native_entity_set_position(this.Id, position);
    }

    getPosition() : Vector3 {
        return Vector3.fromNative(native_entity_get_position(this.Id));
    }

    getForward() : Vector3 {
        return Vector3.fromNative(native_entity_get_forward(this.Id));
    }

    getRight() : Vector3 {
        return Vector3.fromNative(native_entity_get_right(this.Id));
    }

    getUp() : Vector3 {
        return Vector3.fromNative(native_entity_get_up(this.Id));
    }

    applyVelocity(vel : Vector3) : void {
        native_entity_apply_velocity(this.Id.Index, vel.x, vel.y, vel.z);
    }

    GetId() : Guid {
        return this.Id;
    }

    private Id : Guid;
}

declare function native_entity_set_position(index : Guid, pos : Vector3) : void;
declare function native_entity_get_position(index : Guid) : Vector3;
declare function native_entity_get_forward(index : Guid) : Vector3;
declare function native_entity_get_up(index : Guid) : Vector3;
declare function native_entity_get_right(index : Guid) : Vector3;

declare function native_entity_apply_velocity(index : string, x : number,y : number,z : number) : void;