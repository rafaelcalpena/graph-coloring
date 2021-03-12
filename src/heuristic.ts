import {Domain} from './classes';

/* TODO: Add more heuristics */
export const heuristic = <T>(domain: Domain<T>): [string, T] => {
    let result!: [string, T];
    /* Return first literal found */
    for (let [k, v] of domain) {
        result = [k, v];
    }
    return result;
}