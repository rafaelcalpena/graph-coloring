
import { CBidirectionalTupleSet } from './utils/CBidirectionalTupleSet';
import { difference, allDomainsEmpty, filter, union, equals } from './utils/domain-utils';
import { check, DIFFERENT, Operation, OR } from './operations';
import { Constraint, ConstraintDefinition, Domain, Domains, PropagatorTreeNode } from './classes';
import { heuristic } from './heuristic';
import { convertToTupleSet, convertToConstraintList } from './utils/conversions';
import { writeGraphJSON } from './utils/write-graph-json';
import { propagate } from './propagate';
import { writeDataJSON } from './utils/write-data-json';
import { graphToColorConstraints } from './utils/graphToConstraints';
import { australiaGraph } from './utils/australia-graph';
import problems, { Problem } from './problems';

/* Creates a propagator tree in depth-first left-first order */
const simpleGenTree = <T>(
    constraint: Constraint,
    subDomain: Domain<T>, /* values that are in Sor unknown */
    valsInSubDomain: Domain<T> = new CBidirectionalTupleSet() /* values that are known to be in S */
) : PropagatorTreeNode<T> => {
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

    if (equals(newSubDomain, valsIn3)) {
        return new PropagatorTreeNode(
            null,
            null,
            deletions,
            null
        )
    }

    /* Pick a variable and value, and branch */
    const nextLiteral = heuristic(difference(newSubDomain, valsIn3));
    const nextLiteralTupleSet = new CBidirectionalTupleSet([nextLiteral]);
    const leftT = simpleGenTree(constraint, newSubDomain, union(valsIn3, nextLiteralTupleSet));
    const rightT = simpleGenTree(constraint, difference(newSubDomain, nextLiteralTupleSet), valsIn3);
    return new PropagatorTreeNode(
        leftT,
        rightT,
        deletions,
        nextLiteral
    )

}

/* A constraint is entailed with respect to a subdomain list SD if every tuple 
allowed on SD is allowed by the constraint */
const entailed = <T>(constraint: Constraint, domain: Domain<T>) : boolean => {
    /* Check and remove literals */
    const {a, b, op} = constraint;
    const domainA = domain.get(a);
    const domainB = domain.get(b);
    for (let vA of domainA) {
        for (let vB of domainB) {
            if (!check[op](vA, vB)) {
                return false;
            }
        }
    }

    for (let vB of domainB) {
        for (let vA of domainA) {
            if (!check[op](vA, vB)) {
                return false;
            }
        }
    }

    return true;
}

/* Improvement from simpleGenTree */
/* Optimizations: 
- "GenTree always returns Nil when no pruning is performed at the current node and both children are Nil"
- Entailment checker.
*/
const genTree = <T>(
    constraint: Constraint,
    subDomain: Domain<T>, /* values that are in Sor unknown */
    valsInSubDomain: Domain<T> = new CBidirectionalTupleSet() /* values that are known to be in S */
) : PropagatorTreeNode<T> | null => { /* Difference: Can return null */

    if (entailed(constraint, subDomain)) {
        return null;
    }

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

    if (equals(newSubDomain, valsIn3) || entailed(constraint, newSubDomain)) { /* New feature here */
        if (deletions.size === 0) { /* New feature here */
            return null;
        } 
        return new PropagatorTreeNode(
            null,
            null,
            deletions,
            null
        )
    }

    /* Pick a variable and value, and branch */
    const nextLiteral = heuristic(difference(newSubDomain, valsIn3));
    const nextLiteralTupleSet = new CBidirectionalTupleSet([nextLiteral]);
    const leftT = genTree(constraint, newSubDomain, union(valsIn3, nextLiteralTupleSet));
    const rightT = genTree(constraint, difference(newSubDomain, nextLiteralTupleSet), valsIn3);

    if ((leftT === null) && (rightT === null) && (deletions.size === 0)) { /* New Feature here */
        return null;
    }
    
    return new PropagatorTreeNode(
        leftT,
        rightT,
        deletions,
        nextLiteral
    )

}

/* Creates a CSP Instance. Returns one Propagator Tree for each constraint */
const createCSP = <T>(
    constraints: ConstraintDefinition[], 
    domains: Record<string, T[]>
) => {
    let initialDomains = new CBidirectionalTupleSet(convertToTupleSet(domains));
    return convertToConstraintList(constraints)
    .map(c => {
        return (process.env.ALG === 'simple' ? simpleGenTree : genTree)(
        c,
        filter(initialDomains, (i) => c.a === i || c.b === i) /* Include other domains or not? */
    )});
}


let ex = process.env.EX as keyof typeof problems;
let {domains, constraints} : Problem = problems[ex || 'article'];

const main = async () => {

    const csp = createCSP(constraints, domains);
    await writeGraphJSON(csp)
    await writeDataJSON({
        domains, 
        constraints
    })
}

main();