const path = require('path');
const util = require('util');
const fs = require('fs');
const execFile = require('child_process').execFile;

const SECOND = 1000; // 1000ms
const MINUTE = 60 * SECOND;

const allAlgorithms = [
    {name: 'greedy', exact: false},
    {name: 'dsatur', exact: false},
    {name: 'greedy-backtracking', exact: true},
    {name: 'dsatur-backtracking', exact: true},
    {name: 'dsatur-sewell', exact: true},
    {name: 'dsatur-pass-always', exact: true},
    {name: 'dsatur-pass-conditional', exact: true},
    {name: 'dsatur-gac-0', exact: true},
    {name: 'dsatur-gac-1', exact: true},
    {name: 'dsatur-gac-2', exact: true}
];

let TIME_LIMIT = process.env.TIMEOUT ? (process.env.TIMEOUT * 1000) : (10 * MINUTE);
console.log('TIME_LIMIT in ms', TIME_LIMIT)

const testbench = async () => {
    let {graphs, algorithms} = require('./config.js')

    if (process.env.FILES) {
        let filteredGraphs = process.env.FILES.split(',').map(g => g.trim());
        console.log('Filtering graphs: ', filteredGraphs);
        graphs = graphs.filter(g => filteredGraphs.includes(g.name));
    }

    let date = new Date();
    let jobId = process.env.AWS_BATCH_JOB_ID ? `job-${process.env.AWS_BATCH_JOB_ID.replace(/:/g, '_')}-${process.env.AWS_BATCH_JOB_ARRAY_INDEX}-` : ``;
    let basePath = process.env.AWS_BATCH_JOB_ID ? '/mount/efs' : `${__dirname}/../output`;
    let summaryFile = `${basePath}/${jobId}summary-${date.getFullYear()}-${date.getMonth() + 1}-${date.getDate()}_${date.getHours()}-${date.getMinutes()}-${date.getSeconds()}.json`;

    var stream = fs.createWriteStream(summaryFile, {flags:'a'});
    stream.write('{"items": [');

    let filteredAlgorithms = process.env.ALGORITHMS ? process.env.ALGORITHMS.split(',').map(a => a.trim()) : [];

    let algorithmRuns = (process.env.ALGORITHMS ? 
        allAlgorithms.filter(a => filteredAlgorithms.includes(a.name))
        :
        allAlgorithms
    ).filter(i => algorithms.find(j => j.name === i.name))

    /* If running on AWS Batch Job array, filter to only specific job array index */
    if (process.env.AWS_BATCH_JOB_ARRAY_INDEX) {

        if (process.env.AWS_BATCH_JOB_ARRAY_INDEX >= (algorithmRuns.length * graphs.length)) {
            throw new Error('Index for job array is out of range');
        }

        /* Has to come before algorithmRuns because it will have its ordering changed */
        let graphIndex = Math.floor(process.env.AWS_BATCH_JOB_ARRAY_INDEX / algorithmRuns.length);
        graphs = [ graphs[graphIndex] ];
        console.log('graphIndex', graphIndex);
        console.log("AWS BATCH graph", graphs);

        let algIndex = process.env.AWS_BATCH_JOB_ARRAY_INDEX % algorithmRuns.length;
        algorithmRuns = [ algorithmRuns[algIndex] ];
        console.log('algorithmIndex', algIndex)
        console.log("AWS BATCH algorithm", algorithmRuns);
    }                

    for (let i = 0, len = graphs.length; i < len; i++) {

        console.log('Number of graphs to be colored ', i, 'out of', graphs.length)

        let item = graphs[i];

        let filename = item.name;
        console.log(filename)
        
        if (process.env.ALGORITHMS) {
            console.log('Filtering Algorithms: ', algorithmRuns.map(a => a.name));
        }


        let promises = algorithmRuns.map(algorithm => new Promise((resolve, reject) => {
            console.log('executing', algorithm.name)
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