import { CBidirectionalTupleSet } from './CBidirectionalTupleSet';
import { Domain } from "../classes";

/* Methods for performing Domain operations (union, difference, filtering, etc) */
/* TODO: Abstract into reusable library */

export const difference = <T>(d1: Domain<T>, d2: Domain<T>): Domain<T> => {
    /* TODO: Use batcher architecture */
    let result = new CBidirectionalTupleSet(d1);
    result.forEach(([k, v]) => {
        if (d2.has(k, v)) {
            result.delete(k, v);
        }
    });
    return result;
};
export const union = <T>(d1: Domain<T>, d2: Domain<T>): Domain<T> => {
    /* TODO: Use batcher architecture */
    let result = new CBidirectionalTupleSet(d1);
    d2.forEach(([k, v]) => {
        result.add(k, v);
    });
    return result;
};
export const allDomainsEmpty = <T>(domain: Domain<T>) => {
    return domain.size === 0;
};
export const filter = <T>(domain: Domain<T>, fn: (a: string, b: T) => boolean): Domain<T> => {
    let result = new CBidirectionalTupleSet(domain);
    domain.forEach(([k, v]) => {
        if (!fn(k, v)) {
            result.delete(k, v);
        }
    });
    return result;
};
export const equals = <T>(d1: Domain<T>, d2: Domain<T>) : boolean => {
    if (d1.size !== d2.size) {
        return false;
    }
    if ([...d1.firstKeys()].length !== [...d2.firstKeys()].length) {
        return false;
    }
    for (let [k, v] of d1) {
        if (!d2.has(k, v)) {
            return false;
        }
    }
    return true;
}
