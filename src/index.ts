import {BidirectionalTupleSet} from '@stackomate/data-structures';

const DIFFERENT = Symbol('DIFFERENT');

type Operation = typeof DIFFERENT;

/* A class to represent a mathematical variable */
class Variable<T> {
    name: string;
    domain: Set<T>;

    constructor(name: string, domain: Set<T>) {
        this.name = name;
        this.domain = domain;
    }
}

class PropagatorTreeNode<T> {
    constructor(
        public left: PropagatorTreeNode<T> | null,
        public right: PropagatorTreeNode<T> | null,
        public prune: Domain<T>,
        public test: [string, T] | null
    ) {}
}

class Constraint {
    constructor (
        public a: string,
        public op: Operation,
        public b: string
    ) {}
}

type Domain<T> = BidirectionalTupleSet<string, T>;

const propagate = <T>(constraint: Constraint, domain: Domain<T>) : Domain<T> => {
    /* Check and remove literals */
}

const difference = <T>(d1: Domain<T>, d2: Domain<T>) : Domain<T> => {
    
}

const union = <T>(d1: Domain<T>, d2: Domain<T>) : Domain<T> => {

}

const allDomainsEmpty = (domain: Domain<T>) => {
    return domain.size === 0;
}

const heuristic = <T>(domain: Domain<T>) : [string, T] => {
    return /* random */
}

const filter = <T>(domain: Domain<T>, fn: (a: string, b: T) => boolean) : Domain<T> => {

}

/* Creates a propagator tree */
const simpleGenTree = <T>(constraint: Constraint, subDomain: Domain<T>, valsInSubDomain: Domain<T>) : PropagatorTreeNode<T> => {

    /* Get necessary deletions from a positive GAC table propagator (GAC3 ?) */
    const deletions = propagate(constraint, subDomain);

    /* Remove the deletions from the subdomain */
    const newSubDomain = difference(subDomain, deletions); 

    if (allDomainsEmpty(newSubDomain)) {
        return new PropagatorTreeNode(
            null,
            null,
            deletions,
            null
        );
    }

    const valsIn2 = difference(valsInSubDomain, deletions)
    const oneItemVariables = filter(newSubDomain, (k, v) => newSubDomain.get(k).size === 1)/* (x,a)|(x,a)∈ SD′,|SD′(x)|=1 */
    const valsIn3 = union(valsIn2, oneItemVariables);

    if (newSubDomain === valsIn3) {
        return new PropagatorTreeNode(
            null,
            null,
            deletions,
            null
        )
    }

    /* Pick a variable and value, and branch */
    const nextLiteral = heuristic(difference(newSubDomain, valsIn3));
    const nextLiteralTupleSet = new BidirectionalTupleSet([nextLiteral]);
    const leftT = simpleGenTree(constraint, newSubDomain, union(valsIn3, nextLiteralTupleSet));
    const rightT = simpleGenTree(constraint, difference(newSubDomain, nextLiteralTupleSet), valsIn3);
    return new PropagatorTreeNode(
        leftT,
        rightT,
        deletions,
        nextLiteral
    )

}


/* Get the graph */

    /* Create variables */
const variables = [];

/* Create constraints */
const constraints = [];

/* Generate the tree */
