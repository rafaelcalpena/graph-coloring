import { CBidirectionalTupleSet } from './CBidirectionalTupleSet';
import { Constraint, ConstraintDefinition, Domain } from '../classes';

/* Utility function to convert definition into (var, value) literals */
export const convertToTupleSet = <T>(obj: Record<string, T[]>): Domain<T> => {
    let tuples = Object.keys(obj).map(k => obj[k].map(v => [k, v] as [string, T])).flat();
    return new CBidirectionalTupleSet(tuples);
};
export const convertToConstraintList = (cs: ConstraintDefinition[]): Constraint[] => {
    return cs.map(c => new Constraint(
        c[0],
        c[1],
        c[2]
    ));
};
