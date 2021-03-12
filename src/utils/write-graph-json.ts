import { PropagatorTreeNode } from '../classes';
import { createGraphJSON } from './create-graph-json';
import fs from 'fs';
import path from 'path';
import mkdirp from 'mkdirp';

export const writeGraphJSON = async <T>(csp: PropagatorTreeNode<T>[]) => {
    const dataFolder = path.join(__filename, '../../../data/');
    await mkdirp(dataFolder);
    fs.writeFileSync(
        path.join(dataFolder, './propagator-tree.js'),
        `const graphResult = ` + JSON.stringify(
            csp.map(c => createGraphJSON(c)),
            null,
            2
        ),
        'utf8'
    );
};
