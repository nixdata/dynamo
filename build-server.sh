gcc \
-L/home/phil/Code/dynamo/ \
-Wl,-rpath=. \
-Wall \
-o server \
server.cpp \
-ldynamo
