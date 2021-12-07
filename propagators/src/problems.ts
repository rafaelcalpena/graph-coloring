import { ConstraintDefinition, Domains } from "./classes";
import { DIFFERENT, OR } from "./operations";
import { australiaGraph } from "./utils/australia-graph";
import { graphToColorConstraints } from "./utils/graphToConstraints";
import { usaGraph } from "./utils/usa-graph";

export type Problem = {
    domains: Domains,
    constraints: ConstraintDefinition[]
}

/* Problem from the article */
export const article: Problem = {
    domains: {
        x: [0, 1],
        y: [0, 1],
    },
    constraints: [
        ['x', OR, 'y']
    ]
};

export const diff1: Problem = {
    domains: {
        x: [0, 1],
        y: [0, 1],
    },
    constraints: [
        ['x', DIFFERENT, 'y']
    ]
};

export const diff2: Problem = {
    domains: {
        x: [0, 1],
        y: [0, 1],
        z: [1, 3]
    },
    constraints: [
        ['x', DIFFERENT, 'y'],
        ['y', DIFFERENT, 'z']
    ]    
}

export const australia: Problem = graphToColorConstraints(australiaGraph.nodes, australiaGraph.edges);

export const usa : Problem = graphToColorConstraints(usaGraph.nodes, usaGraph.edges);

export default {
    australia,
    diff2,
    diff1,
    article,
    usa
}