gcc \
-Wall \
-Werror \
-O2 \
-fPIC \
-shared \
-o libdynamo.so \
./lib/netcode.io/c/netcode.c \
dynamo.cpp \
dynamo-system.cpp \
dynamo-network.cpp \
