import { PropagatorTreeNode } from '../classes';
import { createGraphJSON } from './create-graph-json';
import fs from 'fs';
import path from 'path';
import mkdirp from 'mkdirp';
import { CBidirectionalTupleSet } from './CBidirectionalTupleSet';

const getNodesLength = <T>(node: PropagatorTreeNode<T> | null) => {
    let count = 0;
    if (node === null) {
        return 0;
    }
    count++;
    if (node.left) {
        count += getNodesLength(node.left);
    }
    if (node.right) {
        count += getNodesLength(node.right)
    }
    return count;
}

const getEdgesLength = <T>(node: PropagatorTreeNode<T> | null) => {
    let count = 0;
    if (node === null) {
        return 0;
    }
    if (node.left) {
        count += getNodesLength(node.left);
    }
    if (node.right) {
        count += getNodesLength(node.right)
    }
    return count;
}

let id = 0;
const createRels = <T>(node: PropagatorTreeNode<T> | null, step: any[] = []): any[] => {

    if (node === null) {
        return step;
    }
    const parentId = id;
    id++;
    if (node.left) {
        step.push([parentId, id]);
        createRels(node.left, step);
    }
    if (node.right) {
        step.push([parentId, id]);
        createRels(node.right, step);
    }
    return step;
}

export const writeGraphJSON = async <T>(csp: (PropagatorTreeNode<T> | null)[]) => {
    const dataFolder = path.join(__filename, '../../../data/');
    await mkdirp(dataFolder);
    fs.writeFileSync(
        path.join(dataFolder, './propagator-tree.js'),
        `const graphResult = ` + JSON.stringify(
            csp.map(c => createGraphJSON(c)),
            null,
            2
        ) + `\nconst info = ` + JSON.stringify({
            nodesLength: csp.map(c => getNodesLength(c)),
            edgesLength: csp.map(c => getEdgesLength(c)),
            rels: csp.map(c => createRels(c))
        }),
        'utf8'
    );
};
