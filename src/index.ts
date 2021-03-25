
import { CBidirectionalTupleSet } from './utils/CBidirectionalTupleSet';
import { difference, allDomainsEmpty, filter, union, equals } from './utils/domain-utils';
import { check, DIFFERENT, Operation } from './operations';
import { Constraint, ConstraintDefinition, Domain, Domains, PropagatorTreeNode } from './classes';
import { heuristic } from './heuristic';
import { convertToTupleSet, convertToConstraintList } from './utils/conversions';
import { writeGraphJSON } from './utils/write-graph-json';
import { propagate } from './propagate';
import { writeDataJSON } from './utils/write-data-json';
import { graphToColorConstraints } from './utils/graphToConstraints';
import { australiaGraph } from './utils/australia-graph';

/* Creates a propagator tree in depth-first left-first order */
const simpleGenTree = <T>(
    constraint: Constraint,
    subDomain: Domain<T>, /* values that are in Sor unknown */
    valsInSubDomain: Domain<T> = new CBidirectionalTupleSet() /* values that are known to be in S */
) : PropagatorTreeNode<T> => {
    console.log(valsInSubDomain.size);
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

/* Creates a CSP Instance. Returns one Propagator Tree for each constraint */
const createCSP = <T>(
    constraints: ConstraintDefinition[], 
    domains: Record<string, T[]>
) => {
    let initialDomains = new CBidirectionalTupleSet(convertToTupleSet(domains));
    return convertToConstraintList(constraints)
    .map(c => {
        return simpleGenTree(
        c,
        filter(initialDomains, (i) => c.a === i || c.b === i) /* Include other domains or not? */
    )});
}

let domains: Domains = {};
let constraints: ConstraintDefinition[];

const australiaExample = () => {
    let info = graphToColorConstraints(australiaGraph.nodes, australiaGraph.edges);
    domains = info.domains;
    constraints = info.constraints;
}

const example1 = () => {
    domains = {
        x: [0, 1],
        y: [0, 1],
        z: [1, 3]
    };
    constraints = [
        ['x', DIFFERENT, 'y'],
        ['y', DIFFERENT, 'z']
    ];
};

const main = async () => {

    example1();
    // australiaExample();

    const csp = createCSP(constraints, domains);
    await writeGraphJSON(csp)
    await writeDataJSON({
        domains, 
        constraints
    })
}

main();