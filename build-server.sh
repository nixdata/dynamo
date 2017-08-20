g++ \
-ggdb \
-L./ \
-Wl,-rpath=./ \
-Wall \
-Werror \
-o server \
./lib/netcode/netcode.c \
server.cpp \
dynamo.cpp \
dynamo-server.cpp \
-lsodium \
-std=c++11
