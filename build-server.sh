gcc \
-L./ \
-Wl,-rpath=./ \
-Wall \
-Werror \
-o server \
server.cpp \
-ldynamo \
-lsodium \
