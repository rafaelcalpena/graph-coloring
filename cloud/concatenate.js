const {execSync} = require('child_process')
const fs = require('fs');

const postProcess = async () => {
    
    /* Copy s3 contents to docker container */
    execSync(`aws s3 cp s3://$(pulumi stack output resultsBucketName -s dev) ./graph-results --recursive`, {stdio: 'inherit'});
    let files = (await fs.promises.readdir('./graph-results')).filter(f => f.startsWith('job-'));

    let batchJobOutputs = new Map();

    /* Iterate over each partial file */
    for (let file of files) {
        /* Get array job id */
        let batchJobId = file.split('_')[0];
        console.log('jobId:', batchJobId);

        /* Create objects for jobId results */
        let mergedContent = batchJobOutputs.get(batchJobId) || {};
        batchJobOutputs.set(batchJobId, mergedContent);

        let graphsInFile = require(`./graph-results/${file}`).items;
        
        /* Merge results */
        for (let g of graphsInFile) {
            /* g is the object with {[filename]: results}. There's only one key in the object */
            let graphFileName = Object.keys(g)[0];

            mergedContent[graphFileName] = {
                ...(mergedContent[graphFileName] || {}),
                ...g[graphFileName]
            };
        }
    }

    console.log(`Generated ${batchJobOutputs.size} batch outputs`);
    
    for (let [key, output] of batchJobOutputs) {
        let finalResult = {items: [output]};
        console.log(key, `has ${Object.keys(output).length} files`)
        let contents = JSON.stringify(finalResult, null, 2);
        await fs.promises.writeFile(`./graph-results/concat-${key}.json`, contents, 'utf-8');
    }

}

postProcess();