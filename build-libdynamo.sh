gcc \
-Wall \
-Werror \
-O2 \
-fPIC \
-shared \
-o libdynamo.so \
-ldl \
./lib/netcode.io/c/netcode.c \
dynamo.cpp \
dynamo-system.cpp \
dynamo-network.cpp \
