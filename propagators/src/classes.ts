import { CBidirectionalTupleSet } from './utils/CBidirectionalTupleSet';
import { Operation } from './operations';

export class PropagatorTreeNode<T> {
    constructor(
        public left: PropagatorTreeNode<T> | null,
        public right: PropagatorTreeNode<T> | null,
        public prune: Domain<T>,
        public test: [string, T] | null
    ) { }
}
export class Constraint {
    constructor(
        public a: string,
        public op: Operation,
        public b: string
    ) { }
}

export type Domain<T> = CBidirectionalTupleSet<string, T>;
export type ConstraintDefinition = [string, Operation, string];
export type Domains = Record<string, number[]>;

