export class Vector4
{
    x : number;
    y : number;
    z : number;
    w : number;

    constructor(x : number = 0.0, y : number = 0.0, z : number = 0.0, w : number = 0.0) {
        this.x = x;
        this.y = y;
        this.z = z;
        this.w = w;
    }

    static fromNative(v : Vector4) : Vector4{
        return new Vector4(v.x,v.y, v.z, v.w);
    }

    equals(other : Vector4) : boolean {
        return other.x == this.x && other.y == this.y && other.z == this.z && other.w == this.w;
    }

    neg() : Vector4
    {
        return new Vector4(-this.x, -this.y, -this.z, -this.w);
    }

    mul(v : Vector4) : Vector4;
    mul(x : number) : Vector3;

    mul(v : Vector4 | number) : Vector4
    {
        if (typeof v == 'number') {
            return new Vector4(this.x * v, this.y * v, this.z * v, this.w * v);
        }
        return new Vector4(this.x * v.x, this.y * v.y, this.z * v.z, this.w * v.w);
    }
}

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

    static fromNative(v : Vector3) : Vector3{
        return new Vector3(v.x,v.y, v.z);
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

    static fromNative(v : Vector2) : Vector2{
        return new Vector2(v.x,v.y);
    }

    equals(other : Vector2) : boolean {
        return other.x == this.x && other.y == this.y;
    }

    neg() : Vector2
    {
        return new Vector2(-this.x, -this.y);
    }

    mul(v : Vector2) : Vector2;
    mul(x : number) : Vector2;

    mul(v : Vector2 | number) : Vector2
    {
        if (typeof v == 'number') {
            return new Vector3(this.x * v, this.y * v);
        }
        return new Vector3(this.x * v.x, this.y * v.y);
    }
}