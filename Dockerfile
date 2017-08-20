FROM ubuntu 


COPY . /dynamo
WORKDIR /dynamo

RUN apt-get update; \
    apt-get -y install --reinstall build-essential; \
    apt-get -y install libsodium-dev 
RUN sh ./build-server.sh 

    
EXPOSE 8080

CMD ["./server", "some_protocol", "127.0.0.1:8080"]
