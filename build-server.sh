gcc \
-v \
-Wl,-rpath=. \
-Werror \
-Wall \
-L./libdynamo \
-lsodium \
-o server \
server.cpp \
-I./
