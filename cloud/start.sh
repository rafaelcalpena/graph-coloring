#!/bin/bash

# Provision resources
pulumi up -y -s dev

# Trigger job start
# Display total count of jobs
echo "Jobs to be submitted: " 
echo $(node -e 'let m = require("../graph-coloring/testbench/config.js"); console.log(m.graphs.length * m.algorithms.length)')

# Wait for 5 minutes to avoid concurrency issues */
echo "waiting for 5 minutes"
sleep 300s
echo "waiting complete"

# Trigger submitted job
aws batch submit-job --job-name partial-test --job-queue $(pulumi stack output jobQueueArn -s dev) --job-definition $(pulumi stack output jobDefinitionArn -s dev) --region us-east-1 --array-properties size=$(node -e 'let m = require("../graph-coloring/testbench/config.js"); console.log(m.graphs.length * m.algorithms.length)')

# Access link to view current state of jobs
echo "https://us-east-1.console.aws.amazon.com/batch/home?region=us-east-1#jobs"

# Once all jobs are complete, user can run datasync.sh script to transfer from EFS to S3 bucket,
# then run concatenate.sh to copy all JSON summary files to local and concatenate the result,
# and finally use convert-to-csv.js to convert to spreadsheet format.