gcc \
-ggdb \
-L./ \
-Wl,-rpath=./ \
-Wall \
-Werror \
-o server \
./lib/netcode.io/c/netcode.c \
server.cpp \
dynamo-system.cpp \
-lsodium \
