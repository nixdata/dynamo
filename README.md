# dynamo
A game engine experiment.

## Build Docker Image
git clone git@github.com:nixdata/dynamo.git
cd dynamo
docker build ./ -t dynamo-server
docker run -e PROTO="45" -d -p 8080:8080 dynamo-server

