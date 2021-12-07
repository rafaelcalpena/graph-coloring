import { BidirectionalTupleSet } from "@stackomate/data-structures";
import { australiaGraph } from "./australia-graph";
import { usaGraph } from "./usa-graph";

type Node = {id: string};
type Edge = {from: string, to: string};
type Graph = {
    nodes: Array<Node>,
    edges: Array<Edge>
}

const incrementCount = (degrees: Map<any, any>, id: string) => {
    degrees.set(id, degrees.has(id) ? degrees.get(id) + 1 : 1);
}

const orderByDegree = (graph: Graph) : Array<[string, number]> => {
    let degrees = new Map<string, number>();

    graph.nodes.forEach(node => {
        degrees.set(node.id, 
            0
        );
    })

    graph.edges.forEach(edge => {
        incrementCount(degrees, edge.from);
        incrementCount(degrees, edge.to);
    })

    return [...degrees].sort((i, j) => -(i[1] - j[1]));

}

const generateSubGraph = (graph: Graph, uncoloredVertices: Set<string>) => {
    let subGraph: Graph = {
        nodes: [],
        edges: []
    };
    graph.nodes.forEach(n => {
        if (uncoloredVertices.has(n.id)) {
            subGraph.nodes.push(n);
        }
    })
    graph.edges.forEach(n => {
        if (uncoloredVertices.has(n.from)) {
            subGraph.edges.push(n);
        }
    })    
    return subGraph;
}

const colorableVertices = (graph: Graph, uncoloredVertices: Set<string>) => {
    if (uncoloredVertices.size > 0) {
        let subGraph = generateSubGraph(graph, uncoloredVertices);
        return orderByDegree(subGraph)
    }
    return [];
}

const getSaturationDegree = (graph: Graph, vertex: string, colorings: BidirectionalTupleSet<any, any>) => {
    let neighbors = new Set();
    graph.edges.forEach(e => {
        if (e.from === vertex) {
            neighbors.add(e.to)
        } else if (e.to === vertex) {
            neighbors.add(e.from)
        }
    })
    let d = 0;
    for (let n of neighbors) {
        if (colorings.inverseGet(n).size > 0) {
            d++;
        } 
    }
    return d;
}

const DSATUR = (graph: Graph) => {

    /* length of vertices */
    let n = graph.nodes.length;
    /* Set of colorings */
    let colorings = new BidirectionalTupleSet();
    /* Order vertices by degree */
    let degreeOrder = orderByDegree(graph);
    /* Vertex with largest no of neighbors */
    let [firstVertex, firstVertexDegree] = degreeOrder[0];
    colorings.add(1, firstVertex);

    let uncoloredVertices = new Set(graph.nodes.slice(1).map(n => n.id));

    let saturationSorted = colorableVertices(graph, uncoloredVertices)
    /* While there are vertices to be colored */
    while (saturationSorted.length > 0) {
        console.log(saturationSorted)
    }

}

DSATUR(australiaGraph)