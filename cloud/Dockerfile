FROM pulumi/pulumi-nodejs

# Install docker because we will use it to build another image in Pulumi
# Use apt-get instead of apt: https://askubuntu.com/questions/990823/apt-gives-unstable-cli-interface-warning
RUN apt-get update
RUN apt-get install -y curl
RUN curl -fsSL https://get.docker.com -o get-docker.sh
RUN sh get-docker.sh

# When running this container, remember to provide access key environment variables
# AWS_ACCESS_KEY_ID and AWS_SECRET_ACCESS_KEY
# PULUMI_CONFIG_PASSPHRASE
# Also mount volume to ~/.pulumi for local access

RUN npm i -g pnpm

# Install aws cli
RUN curl "https://awscli.amazonaws.com/awscli-exe-linux-x86_64.zip" -o "awscliv2.zip"
RUN apt-get install unzip
RUN unzip awscliv2.zip
RUN ./aws/install
RUN aws --version

WORKDIR ./cloud

# pnpm fetch only needs the lockfile
COPY ./pnpm-lock.yaml ./pnpm-lock.yaml

RUN pnpm fetch

COPY . .

RUN ls -a

RUN pnpm install -r --offline

RUN chmod +x ./concatenate.sh ./start.sh ./datasync.sh

# save state locally in mounted volume
# https://www.pulumi.com/docs/intro/concepts/state/#logging-into-the-local-filesystem-backend
CMD pulumi login --local && \
    tail -f /dev/null