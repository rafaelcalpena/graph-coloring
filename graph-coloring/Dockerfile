FROM gcc:latest as compiler
ARG SOURCE_FOLDER="./"
WORKDIR ~

# Install Node.js
RUN curl -fsSL https://deb.nodesource.com/setup_17.x | bash -
RUN apt-get install -y nodejs

COPY ${SOURCE_FOLDER} .

# Use g++ for ubuntu
# -stdlib=libc++ is not present for g++
# cannot use clear otherwise TERM environment variable not set.
# Add to g++ compilation for stricter outputs: -Wall -Wextra -Werror 
#RUN g++ --version && g++ -std=c++2a -fmax-errors=5  src/graph-coloring.cpp -o ./run 
RUN g++ --version && g++ -std=c++2a -fmax-errors=5  src/gac-network/gac-network.cpp -o ./run 

# Copy compiled binary from previous stage
# CMD node --version && DEBUG=0 FILE="dimacs/queen7_7.col" ALG="dsatur-backtracking" ./run
# CMD node ./testbench
CMD ./run