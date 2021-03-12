import { ConstraintDefinition, Domains, PropagatorTreeNode } from '../classes';
import { createGraphJSON } from './create-graph-json';
import fs from 'fs';
import path from 'path';
import mkdirp from 'mkdirp';

export const writeDataJSON = async <T>(obj: {domains: Domains, constraints: ConstraintDefinition[]}) => {
    const dataFolder = path.join(__filename, '../../../data/');
    await mkdirp(dataFolder);
    fs.writeFileSync(
        path.join(dataFolder, './data.js'),
        `const dataResult = ` + JSON.stringify(
            obj,
            null,
            2
        ),
        'utf8'
    );
};
