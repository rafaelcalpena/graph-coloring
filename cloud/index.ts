import * as pulumi from "@pulumi/pulumi";
import * as aws from "@pulumi/aws";
import * as awsx from "@pulumi/awsx";

/* Sources: https://www.pulumi.com/docs/reference/pkg/nodejs/pulumi/awsx/ec2/,
https://cj-hewett.medium.com/connect-an-aws-ecs-fargate-service-to-an-efs-file-system-with-pulumi-4f984d371a9b */

// Create vpc to be used by resources below
const vpc = new awsx.ec2.Vpc(`graphColoring-vpc`, {
    cidrBlock: "10.0.0.0/16",
    numberOfAvailabilityZones: 1,
    /* TODO: This is probably not safe, but private subnets are more expensive due to NAT */
    subnets: [{ type: "public" }],
});
/* Required by the nfs transfer protocol for efs */
const NFS_PORT = 2049;

// Export a few resulting fields to make them easy to use:
export const vpcId = vpc.id;
export const vpcPublicSubnetIds = vpc.publicSubnetIds;

export const publicSubnet = pulumi.output(vpc.publicSubnetIds)[0];

// Allocate a security group and then a series of rules:
const sg = new awsx.ec2.SecurityGroup(`graphColoring-sg`, { vpc });
// inbound nfs traffic on port 2049 from a specific IP address
sg.createIngressRule("nfs-access", {
  location: new awsx.ec2.AnyIPv4Location(),
  ports: new awsx.ec2.TcpPorts(NFS_PORT),
  description: "allow NFS access for EFS from anywhere",
});

// outbound TCP traffic on any port to anywhere
sg.createEgressRule("outbound-access", {
  location: new awsx.ec2.AnyIPv4Location(),
  ports: new awsx.ec2.AllTcpPorts(),
  description: "allow outbound access to anywhere",
});

// Create EFS resource to save coloring results
const efs = new aws.efs.FileSystem("graphColoring");

// Create a mount target for both public subnets
const privateMountTarget = new aws.efs.MountTarget(`graphColoring-privateMountTarget`, {
    fileSystemId: efs.id,
    subnetId: publicSubnet,
    securityGroups: [sg.id]
  });

const resultsBucket = new aws.s3.Bucket("graph-coloring-results", {
    acl: "private",
    tags: {
        Environment: "Dev",
        Name: "Graph Coloring Results",
    },
    /* Destroy s3 bucket even if it has files */
    forceDestroy: true
});

export const resultsBucketName = resultsBucket.bucket;

/* Check: https://github.com/pulumi/pulumi-awsx/issues/661#issuecomment-1030361075 */

/* AWS Batch */
/* Create compute environment */
/* See https://www.pulumi.com/registry/packages/aws/api-docs/batch/computeenvironment/ */
const graphColoringEnvironment = new aws.batch.ComputeEnvironment("graphColoringEnvironment", {
    computeEnvironmentName: "graphColoringEnvironment",
    computeResources: {
        maxVcpus: 1024,
        securityGroupIds: [sg.id],
        subnets: [publicSubnet],
        type: "FARGATE",
    },
    type: "MANAGED",
}, {});

const graphColoringJobQueue = new aws.batch.JobQueue("graphColoringJobQueue", {
    state: "ENABLED",
    priority: 1,
    computeEnvironments: [
        graphColoringEnvironment.arn
    ],
});

export const jobQueueArn = graphColoringJobQueue.arn;

const assumeRolePolicy = aws.iam.getPolicyDocument({
    statements: [{
        actions: ["sts:AssumeRole"],
        principals: [{
            type: "Service",
            identifiers: ["ecs-tasks.amazonaws.com"],
        }, 
        {
            type: "Service",
            identifiers: ["s3.amazonaws.com"],
        }, {
            type: "Service",
            identifiers: ["datasync.amazonaws.com"],
        }],
    }],
});
const ecsTaskExecutionRole = new aws.iam.Role("ecsTaskExecutionRole", {assumeRolePolicy: assumeRolePolicy.then(assumeRolePolicy => assumeRolePolicy.json)});
const ecsTaskExecutionRolePolicy = new aws.iam.RolePolicyAttachment("ecsTaskExecutionRolePolicy", {
    role: ecsTaskExecutionRole.name,
    policyArn: "arn:aws:iam::aws:policy/service-role/AmazonECSTaskExecutionRolePolicy",
});

const repo = new awsx.ecr.Repository("graph-coloring-repo");

const image = repo.buildAndPushImage("../graph-coloring");

const graphColoringJobDefinition = new aws.batch.JobDefinition("graphColoringJobDefinition", {
    type: "container",
    platformCapabilities: ["FARGATE"],
    containerProperties: pulumi.interpolate`{
  "networkConfiguration": { 
    "assignPublicIp": "ENABLED"
  },
  "image": "${image}",
  "fargatePlatformConfiguration": {
    "platformVersion": "1.4.0"
  },
  "resourceRequirements": [
    {"type": "VCPU", "value": "0.25"},
    {"type": "MEMORY", "value": "512"}
  ],
  "executionRoleArn": "${ecsTaskExecutionRole.arn}",
  "mountPoints": [
    {
      "containerPath": "/mount/efs",
      "sourceVolume": "graphColoring-volume"
    }
  ],
  "volumes": [
        {
            "name": "graphColoring-volume",
            "efsVolumeConfiguration": {
            "fileSystemId": "${privateMountTarget.fileSystemId}",
            "transitEncryption": "ENABLED"
            }
        }
    ],
  "environment": [
      {
          "name": "TIMEOUT",
          "value": "3600"
      }
  ]
}
`,
});

export const jobDefinitionArn = graphColoringJobDefinition.arn;

const efsLocation = new aws.datasync.EfsLocation("efsLocation", {
    efsFileSystemArn: privateMountTarget.fileSystemArn,
    ec2Config: {
        securityGroupArns: [sg.securityGroup.arn],
        /* TODO: Typing */
        subnetArn: pulumi.output(vpc.publicSubnets)[0].apply(s => s.subnet.arn),
    },
});

const s3AccessRole = new aws.iam.Role("s3AccessRole", {assumeRolePolicy: assumeRolePolicy.then(assumeRolePolicy => assumeRolePolicy.json)});
const s3AccessRolePolicy = new aws.iam.RolePolicyAttachment("s3AccessRolePolicy", {
    role: s3AccessRole.name,
    policyArn: "arn:aws:iam::aws:policy/AmazonS3FullAccess",
});

const s3Location = new aws.datasync.S3Location("s3Location", {
    s3BucketArn: resultsBucket.arn,
    s3Config: {
        bucketAccessRoleArn: s3AccessRole.arn,
    },
    subdirectory: '/'
});


const graphColoringDataSync = new aws.datasync.Task("graphColoringDataSync", {
    sourceLocationArn: efsLocation.arn,
    destinationLocationArn: s3Location.arn
});

export const dataSyncTaskArn = graphColoringDataSync.arn;