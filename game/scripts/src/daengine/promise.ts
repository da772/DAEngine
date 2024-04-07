export class PromiseCompletionSource<T>
{
    private _promise : Promise<T>;
    private _resolve : (T) => void;
    private _reject : () => void;

    constructor() {
        this._promise = new Promise<T>((resolve, reject) => {
            this._resolve = resolve;
            this._reject = reject;
        });
    }

    Promise() : Promise<T> {
        return this._promise;
    }

    Resolve(val : T) : void {
        this._resolve(val);
    }

    Reject() : void {
        this._reject();
    }
}

export let promise = new PromiseCompletionSource();