export class Vector3
{
    x : number;
    y : number;
    z : number;

    constructor(x : number = 0.0, y : number = 0.0, z : number = 0.0) {
        this.x = x;
        this.y = y;
        this.z = z;
    }

    equals(other : Vector3) : boolean {
        return other.x == this.x && other.y == this.y && other.z == this.z;
    }

    neg() : Vector3
    {
        return new Vector3(-this.x, -this.y, -this.z);
    }

    mul(v : Vector3) : Vector3;
    mul(x : number) : Vector3;

    mul(v : Vector3 | number) : Vector3
    {
        if (typeof v == 'number') {
            return new Vector3(this.x * v, this.y * v, this.z * v);
        }
        return new Vector3(this.x * v.x, this.y * v.y, this.z * v.z);
    }
}


export class Vector2
{
    x : number;
    y : number;

    constructor(x : number = 0.0, y : number = 0.0) {
        this.x = x;
        this.y = y;
    }

    equals(other : Vector2) : boolean {
        return other.x == this.x && other.y == this.y;
    }

    neg() : Vector3
    {
        return new Vector3(-this.x, -this.y);
    }

    mul(v : Vector3) : Vector3;
    mul(x : number) : Vector3;

    mul(v : Vector3 | number) : Vector3
    {
        if (typeof v == 'number') {
            return new Vector3(this.x * v, this.y * v);
        }
        return new Vector3(this.x * v.x, this.y * v.y);
    }
}