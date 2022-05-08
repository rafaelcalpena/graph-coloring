#!/bin/bash

# Trigger job start
aws datasync start-task-execution --task-arn $(pulumi stack output dataSyncTaskArn -s dev) --region us-east-1

echo "https://us-east-1.console.aws.amazon.com/datasync/home?region=us-east-1#/tasks"