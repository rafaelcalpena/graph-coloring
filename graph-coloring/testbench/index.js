const path = require('path');
const util = require('util');
const fs = require('fs');
const execFile = require('child_process').execFile;

const SECOND = 1000; // 1000ms
const MINUTE = 60 * SECOND;


let TIME_LIMIT = 10 * MINUTE;

const testbench = async () => {
    let {graphs, algorithms} = require('./config.js')
    let date = new Date();
    let summaryFile = `${__dirname}/../output/summary-${date.getFullYear()}-${date.getMonth() + 1}-${date.getDate()}_${date.getHours()}-${date.getMinutes()}-${date.getSeconds()}.json`;

    var stream = fs.createWriteStream(summaryFile, {flags:'a'});
    stream.write('{"items": [');

    for (let i = 0, len = graphs.length; i < len; i++) {

        console.log('Number of graphs to be colored ', i, 'out of', graphs.length)

        let item = graphs[i];

        let filename = item.name;
        console.log(filename)
        const algorithmRuns = ([
            {name: 'greedy', exact: false},
            {name: 'dsatur', exact: false},
            {name: 'greedy-backtracking', exact: true},
            {name: 'dsatur-backtracking', exact: true},
            {name: 'dsatur-sewell', exact: true},
            {name: 'dsatur-pass-always', exact: true},
            {name: 'dsatur-pass-conditional', exact: true},
            {name: 'dsatur-gac', exact: true}
        ]).filter(i => algorithms.find(j => j.name === i.name))
        
        let promises = algorithmRuns.map(algorithm => new Promise((resolve, reject) => {
            let hasFinished = false;
            let timeoutHandle = null;
            let childProcess = execFile('./run', {
                env: {
                    /* Disable debugging log */
                    DEBUG: 0,
                    FILE: filename,
                    ALG: algorithm.name
                }
            }, 
            (err, stdout, stderr) => {
                hasFinished = true;
                if (err) {
                    resolve({[algorithm.name]: {timeout: true, time: TIME_LIMIT}});
                    return;
                }
                
                let parsedOutput = JSON.parse(stdout);
                console.log(parsedOutput)
                clearTimeout(timeoutHandle);
                resolve(parsedOutput)
            })

            timeoutHandle = setTimeout(() => {
                if (!hasFinished) {
                    console.log(filename, algorithm.name, 'did not finish after', TIME_LIMIT, 'ms')
                    childProcess.kill();
                    hasFinished = true;
                }
            }, TIME_LIMIT)
        }))

        let results = await Promise.all(promises);
        let finalObj = {}

        finalObj[filename] = {
            info: item
        };

        for (let r of results) {
         
            /* Verify in case algorithm is exact that the coloring is optimal */
            for (let alg of Object.keys(r)) {
                if (algorithmRuns.find(i => i.name === alg).exact) {
                    console.log(`algorithm ${alg} is exact`)
                    if (typeof r[alg].colors !== 'number') {
                        console.log('skipping optimality check because of timeout');
                        continue;
                    }

                    if (typeof finalObj[filename].info.optimal === 'number') {
                        console.log('contains optimal coloring defined', finalObj[filename].info.optimal)
                        if (finalObj[filename].info.optimal !== r[alg].colors) {
                            throw new Error(`Invalid optimal coloring for exact algorithm: ${r[alg].colors} should have been ${finalObj[filename].info.optimal}`)
                        }
                    }
                }
            }

            finalObj[filename] = {
                ...finalObj[filename],
                ...r
            }
        }

        stream.write(JSON.stringify(finalObj, null, 2));
        if (i + 1 < len) {
            stream.write(', ');
        }
    }

    stream.write(']}');
    stream.end();    

}

testbench();