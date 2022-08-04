#!/bin/bash

# This script will sync contents created on an EFS volume to an S3 volume

# Trigger job start
aws datasync start-task-execution --task-arn $(pulumi stack output dataSyncTaskArn -s dev) --region us-east-1

echo "https://us-east-1.console.aws.amazon.com/datasync/home?region=us-east-1#/tasks"