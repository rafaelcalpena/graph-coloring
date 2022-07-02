const { Parser } = require('json2csv');

/* Concatenated file in JSON format to be analyzed */
const file = require(process.env.FILE);

let getAlgObject = (item, alg) => ({
    [`${alg}-time`]: item[alg].timeout ? undefined : item[alg].time,
    [`${alg}-bkt-vertices`]:  item[alg].backtrackingVertices
});

let getAlgHeuristicObject = (item, alg) => ({
    [`${alg}-time`]: item[alg].time,
    [`${alg}-colors`]:  item[alg].colors
});


let transformedJSON = Object.entries(file.items[0]).map(([key, item]) => {
    return {
        'graph-name': key, 
        'optimal-colors': item.info.optimal,
        
        ...getAlgHeuristicObject(item, 'greedy'),
        ...getAlgHeuristicObject(item, 'dsatur'),
        ...getAlgObject(item, 'greedy-backtracking'),
        ...getAlgObject(item, 'dsatur-backtracking'),
        ...getAlgObject(item, 'dsatur-sewell'),
        ...getAlgObject(item, 'dsatur-pass-always'),
        ...getAlgObject(item, 'dsatur-pass-conditional'),
        ...getAlgObject(item, 'dsatur-gac-0'),
        ...getAlgObject(item, 'dsatur-gac-1'),
        ...getAlgObject(item, 'dsatur-gac-2')
    }
})

const main = () => {
    try {
      const parser = new Parser();
      const csv = parser.parse(transformedJSON);
      require('fs').writeFileSync('results.csv', csv);
      console.log(csv);
    } catch (err) {
      console.error(err);
    }
}

main();