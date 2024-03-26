export class Guid
{
    constructor(index : string) {
        this.Index = index;
    }

    Index : string;
}

export let guid = new Guid("");