FROM gcc:latest as compiler

WORKDIR ~

# Install Node.js
RUN curl -fsSL https://deb.nodesource.com/setup_17.x | bash -
RUN apt-get install -y nodejs

COPY . .

# Use g++ for ubuntu
RUN g++ --version && g++ -std=c++2a -fmax-errors=5 src/graph-coloring.cpp -o ./run 

CMD node ./testbench