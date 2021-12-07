import {Domain} from './classes';

/* TODO: Add more heuristics */
export const getFirstHeuristic = <T>(domain: Domain<T>): [string, T] => {
    let result!: [string, T];
    /* Return first literal found */
    for (let [k, v] of domain) {
        result = [k, v];
    }
    return result;
}

export const sortedAlphabetically = <T>(domain: Domain<T>): [string, T] => {
    let variable = [...domain.firstKeys()].sort()[0];
    return [variable, [...domain.get(variable)].sort()[0]];
}

/* Smallest Maybe First (SMF) selects a variable with the smallest non-zero number 
of literals in SD′ \ ValsIn′ */
/* TODO: Abstract to Bidirectional Tuple Set */
export const SMF = <T>(domain: Domain<T>): [string, T] => {
    let sorted = [...domain.firstKeys()]
        .map(key => ({key, size: domain.get(key).size}))
        .filter(d => d.size > 0)
        .sort((a,b) => a.size-b.size);
    let smallestLiteral = [...domain.get(sorted[0].key)].sort()[0];
    return [sorted[0].key, smallestLiteral];
}

export const random = <T>(domain: Domain<T>): [string, T] => {
    let domainArr = [...domain];
    return domainArr[Math.floor(Math.random() * domainArr.length)]
}

export const heuristic = (() => {
    switch (process.env.HEU) {
        case 'first':
            return getFirstHeuristic;
        case 'SMF':
            return SMF;
        case 'random':
            return random;
        case 'alphabetical':
        default:
            return sortedAlphabetically;
    }
})();