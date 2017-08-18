gcc \
-L./ \
-Wl,-rpath=./ \
-Wall \
-Werror \
-o client \
./lib/netcode/netcode.c \
client.cpp \
dynamo.cpp \
dynamo-client.cpp \
-lsodium \
