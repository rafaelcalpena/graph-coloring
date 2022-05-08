#!/bin/bash

# Provision resources
pulumi up -y -s dev

# Trigger job start
# TODO: Use dynamic size here
echo "Jobs to be submitted: " 
echo $(node -e 'let m = require("../graph-coloring/testbench/config.js"); console.log(m.graphs.length * m.algorithms.length)')

aws batch submit-job --job-name partial-test --job-queue $(pulumi stack output jobQueueArn -s dev) --job-definition $(pulumi stack output jobDefinitionArn -s dev) --region us-east-1 --array-properties size=$(node -e 'let m = require("../graph-coloring/testbench/config.js"); console.log(m.graphs.length * m.algorithms.length)')

echo "https://us-east-1.console.aws.amazon.com/batch/home?region=us-east-1#jobs"