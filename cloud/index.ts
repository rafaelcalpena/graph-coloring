import * as pulumi from "@pulumi/pulumi";
import * as aws from "@pulumi/aws";
import * as awsx from "@pulumi/awsx";

/* Sources
- https://www.pulumi.com/docs/reference/pkg/nodejs/pulumi/awsx/ec2/,
- https://cj-hewett.medium.com/connect-an-aws-ecs-fargate-service-to-an-efs-file-system-with-pulumi-4f984d371a9b
- https://github.com/pulumi/pulumi-awsx/issues/661#issuecomment-1030361075 
- https://www.pulumi.com/registry/packages/aws/api-docs/batch/computeenvironment/ */

/* Create vpc to be used by resources below */
const vpc = new awsx.ec2.Vpc(`graphColoring-vpc`, {
    cidrBlock: "10.0.0.0/16",
    numberOfAvailabilityZones: 1,
    /* Trade-off between safety and price: private subnets are much more expensive due to NAT */
    subnets: [{ type: "public" }],
});

/* Required by the nfs transfer protocol for EFS */
const NFS_PORT = 2049;

/* Export a few resulting fields */
export const vpcId = vpc.id;
export const vpcPublicSubnetIds = vpc.publicSubnetIds;
export const publicSubnet = pulumi.output(vpc.publicSubnetIds)[0];

/* Allocate a security group and a series of rules */
const sg = new awsx.ec2.SecurityGroup(`graphColoring-sg`, { vpc });
/* inbound nfs traffic on port 2049 */
sg.createIngressRule("nfs-access", {
  location: new awsx.ec2.AnyIPv4Location(),
  ports: new awsx.ec2.TcpPorts(NFS_PORT),
  description: "allow NFS access for EFS from anywhere",
});

/* outbound TCP traffic on any port to anywhere */
sg.createEgressRule("outbound-access", {
  location: new awsx.ec2.AnyIPv4Location(),
  ports: new awsx.ec2.AllTcpPorts(),
  description: "allow outbound access to anywhere",
});

/* Create EFS resource to save graph coloring results */
const efs = new aws.efs.FileSystem("graphColoring");

/* Create a mount target to be used by EFS volume for public subnets */
const privateMountTarget = new aws.efs.MountTarget(`graphColoring-privateMountTarget`, {
    fileSystemId: efs.id,
    subnetId: publicSubnet,
    securityGroups: [sg.id]
  });

/* Create a S3 Bucket that will act as a bridge for transferring 
between the EFS volume and local environment */
const resultsBucket = new aws.s3.Bucket("graph-coloring-results", {
    acl: "private",
    tags: {
        Environment: "Dev",
        Name: "Graph Coloring Results",
    },
    /* Force Destroying s3 bucket even if it has files on destroy trigger */
    forceDestroy: true
});

/* Export bucket name for later use */
export const resultsBucketName = resultsBucket.bucket;

/* Using AWS Batch to define the compute environment */
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

/* Define Job Queue for the compute environment */
const graphColoringJobQueue = new aws.batch.JobQueue("graphColoringJobQueue", {
    state: "ENABLED",
    priority: 1,
    computeEnvironments: [
        graphColoringEnvironment.arn
    ],
});

/* Export jobQueue identification for later use */
export const jobQueueArn = graphColoringJobQueue.arn;

/* Create required policies and roles to run the tasks */
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

/* Create a container repository that will host the graph coloring container image */
const repo = new awsx.ecr.Repository("graph-coloring-repo");
/* Build and push container image from ../graph-coloring/Dockerfile to the remote repository */
const image = repo.buildAndPushImage("../graph-coloring");

/* Create job definition for running graph coloring algorithm benchmarks.
Using Fargate platform to delegate management of host machines to the cloud provider.
Each container will run with 0.25 VCPU and 512MB RAM, will be connected to the same EFS volume
and contains a TIMEOUT value of 1 hour */
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
/* Export job definition identification for later use */
export const jobDefinitionArn = graphColoringJobDefinition.arn;

/* Configure efsLocation for syncing between EFS and S3 Bucket */
const efsLocation = new aws.datasync.EfsLocation("efsLocation", {
    efsFileSystemArn: privateMountTarget.fileSystemArn,
    ec2Config: {
        securityGroupArns: [sg.securityGroup.arn],
        /* TODO: Typing */
        subnetArn: pulumi.output(vpc.publicSubnets)[0].apply(s => s.subnet.arn),
    },
});

/* Create necessary roles and policies to access S3 Bucket */
const s3AccessRole = new aws.iam.Role("s3AccessRole", {assumeRolePolicy: assumeRolePolicy.then(assumeRolePolicy => assumeRolePolicy.json)});
const s3AccessRolePolicy = new aws.iam.RolePolicyAttachment("s3AccessRolePolicy", {
    role: s3AccessRole.name,
    policyArn: "arn:aws:iam::aws:policy/AmazonS3FullAccess",
});

/* Configure S3Location for syncing between EFS and S3 Bucket */
const s3Location = new aws.datasync.S3Location("s3Location", {
    s3BucketArn: resultsBucket.arn,
    s3Config: {
        bucketAccessRoleArn: s3AccessRole.arn,
    },
    subdirectory: '/'
});

/* Create DataSync task that will be triggered manually by user once test is complete */
const graphColoringDataSync = new aws.datasync.Task("graphColoringDataSync", {
    sourceLocationArn: efsLocation.arn,
    destinationLocationArn: s3Location.arn
});

/* Export dataSyncTask identification for later use */
export const dataSyncTaskArn = graphColoringDataSync.arn;