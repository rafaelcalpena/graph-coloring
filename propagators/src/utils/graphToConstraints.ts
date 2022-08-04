import { ConstraintDefinition, Domains } from "../classes";
import COLORS from "../colors";
import { DIFFERENT } from "../operations";

export const graphToColorConstraints = (nodes: any[], edges: any[]) => {
    let constraints: ConstraintDefinition[] = [];
    let domains: Domains = {};
    edges.forEach(edge => {
        constraints.push([edge.from, DIFFERENT, edge.to]);
    })
    nodes.forEach(node => {
        domains[node.id] = Array.from(Array(COLORS).keys())
    })
    return {
        constraints,
        domains
    }
}