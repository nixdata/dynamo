gcc \
-L./ \
-Wl,-rpath=./ \
-Wall \
-Werror \
-o client \
./lib/netcode.io/c/netcode.c \
client.cpp \
dynamo-system.cpp \
-lsodium \
