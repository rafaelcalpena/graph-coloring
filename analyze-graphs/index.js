const { readFile, readdir, writeFile } = require('fs/promises');
const { resolve, basename } = require('path');

const main = async ([_1, _2, dir]) => {
    const files = (await readdir(dir)).filter(i => !(i.startsWith('.'))).map(f => resolve(dir, f));
   
    let allData = await Promise.all(files.map(f => getGraphData(f)));

    const csvRows = allData.map(obj => ([
        obj.filename,
        obj.declaredVertices,
        obj.realEdges,
        obj.graphDensity,
        obj.minimumDegree,
        obj.maximumDegree
    ]));
    const csvData = [
        ['name', 'vertices', 'edges', 'density', 'minimumDegree', 'maximumDegree'],
        ...csvRows
    ];
    await writeFile(`${__dirname}/export.csv`, csvData.map(row => row.join(',')).join('\n'))
}

const getGraphData = async (filename) => {
    const fileContents = await readFile(filename, 'utf-8');
    const fileLines = fileContents.split('\n');

    /* map edges into sets */
    const adjacencies = {};
    const problemDefinitionLine = fileLines.filter(l => l.startsWith('p edge '))[0].split('p edge ')[1];
    const edges = fileLines.filter(l => l.startsWith('e ')).map(l => l.split('e ')[1]).map(i => i.split(' ').map(j => Number(j)));

    const declaredVertices = Number(problemDefinitionLine.split(' ')[0]);
    const declaredEdges = Number(problemDefinitionLine.split(' ')[1]);

    /* populate adjacencies object */
    for (let i = 1; i <= declaredVertices; i++ ) {
        adjacencies[i] = new Set();
    }

    for (let [from, to] of edges) {
        adjacencies[from].add(to);
        adjacencies[to].add(from);
        /* Only count once */
    }

    let realEdges = Object.entries(adjacencies).map(([k, neighbors]) => neighbors).reduce((acc, neighbors) => acc + neighbors.size, 0) / 2;

    let graphDensity = (2 * realEdges) / (declaredVertices * (declaredVertices - 1));

    let minimumDegree = Object.values(adjacencies).reduce((acc, arr) => Math.min(acc, arr.size), Infinity);
    let maximumDegree = Object.values(adjacencies).reduce((acc, arr) => Math.max(acc, arr.size), 0);

    return {
        filename: basename(filename), 
        declaredVertices, 
        declaredEdges,
        realEdges,
        graphDensity,
        minimumDegree,
        maximumDegree
    };

}

main(process.argv);

