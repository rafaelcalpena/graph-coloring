import { BidirectionalTupleSet } from '@stackomate/data-structures';
import util from 'util';

/* Hack to improve console logging for CBidirectionalTupleSet.
 * The real BidirectionalTupleSet is in @stackomate/data-structures
/* TODO: Abstract into library */
export class CBidirectionalTupleSet<K, V> extends BidirectionalTupleSet<K, V> {
    constructor(iterable?: Iterable<[K, V]>) {
        super(iterable);
    }

    /* TODO: Improve Notations */
    [util.inspect.custom](depth: number, opts: any) {
        return (this).toArray();
    }

    toString() {
        return `{${this.toArray().map(([k, v]) => `(${k},${v})`).toString()}}`;
    }
}
