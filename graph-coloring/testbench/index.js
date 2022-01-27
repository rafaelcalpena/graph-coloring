const path = require('path');
const util = require('util');
const fs = require('fs');
const execFile = require('child_process').execFile;

const SECOND = 1000; // 1000ms
const MINUTE = 1 * SECOND;


let TIME_LIMIT = 1 * MINUTE;

const testbench = async () => {
    let config = require('./config.js')
    let date = new Date();
    let summaryFile = `${__dirname}/../output/summary-${date.getFullYear()}-${date.getMonth() + 1}-${date.getDate()}_${date.getHours()}-${date.getMinutes()}-${date.getSeconds()}.json`;

    var stream = fs.createWriteStream(summaryFile, {flags:'a'});
    stream.write('{"items": [');

    for (let i = 0, len = config.length; i < len; i++) {

        let item = config[i];

        let file = item;
        console.log(file)
        const algorithmRuns = [
            'greedy',
            'dsatur',
            'greedy-backtracking',
            'dsatur-backtracking'
        ]
        
        let promises = algorithmRuns.map(algorithm => new Promise((resolve, reject) => {
            let hasFinished = false;
            let childProcess = execFile('./run', {
                env: {
                    /* Disable debugging log */
                    DEBUG: 0,
                    FILE: file,
                    ALG: algorithm
                }
            }, 
            (err, stdout, stderr) => {
                hasFinished = true;
                if (err) {
                    resolve({[algorithm]: {timeout: true, time: TIME_LIMIT}});
                    return;
                }
                
                let parsedOutput = JSON.parse(stdout);
                console.log(parsedOutput)
                resolve(parsedOutput)
            })

            setTimeout(() => {
                if (!hasFinished) {
                    console.log(file, algorithm, 'did not finish after', TIME_LIMIT, 'ms')
                    childProcess.kill();
                    hasFinished = true;
                }
            }, TIME_LIMIT)
        }))

        let results = await Promise.all(promises);
        let finalObj = {
            [item]: {
                ...results[0],
                ...results[1],
                ...results[2],
                ...results[3]
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