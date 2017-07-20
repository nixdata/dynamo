CFLAGS  = -std=c++11 -pedantic -Wall -Werror -O2 -fPIC 
LDLIBS  = -ldl

all : server libdynamo.so

server : server.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)

libdynamo.so : dynamo.cpp dynamo-system.h dynamo-system.cpp
	$(CC) $(CFLAGS) -shared $(LDFLAGS) -o $@ $< $(LDLIBS)

test : server libdynamo.so
	./$<

clean :
	$(RM) *.o *.so server
