gcc \
-ggdb \
-L./ \
-Wl,-rpath=./ \
-Wall \
-Werror \
-o server \
./lib/netcode/netcode.c \
server.cpp \
dynamo-system.cpp \
dynamo-server.cpp \
-lsodium \
