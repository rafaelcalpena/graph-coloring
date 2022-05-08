#!/bin/bash

# Provision resources
pulumi up -y -s dev

# Trigger job start
# TODO: Use dynamic size here
aws batch submit-job --job-name partial-test --job-queue $(pulumi stack output jobQueueArn -s dev) --job-definition $(pulumi stack output jobDefinitionArn -s dev) --region us-east-1 --array-properties size=9

echo "https://us-east-1.console.aws.amazon.com/batch/home?region=us-east-1#jobs"