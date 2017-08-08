gcc \
-L/home/phil/Code/dynamo/ \
-Wl,-rpath=. \
-Werror \
-Wall \
-o server \
server.cpp \
-ldynamo \
-lsodium \
