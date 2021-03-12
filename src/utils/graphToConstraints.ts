import { ConstraintDefinition, Domains } from "../classes";
import { DIFFERENT } from "../operations";

export const graphToColorConstraints = (nodes: any[], edges: any[]) => {
    let constraints: ConstraintDefinition[] = [];
    let domains: Domains = {};
    edges.forEach(edge => {
        constraints.push([edge.from, DIFFERENT, edge.to]);
    })
    nodes.forEach(node => {
        domains[node.id] = [0, 1, 2, 3]
    })
    return {
        constraints,
        domains
    }
}