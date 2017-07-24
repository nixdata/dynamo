CFLAGS  = -std=c++11 -pedantic -Wall -O2 -fPIC 
LDLIBS  = -ldl

all : main libdynamo.so

main : server.cpp dynamo.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)

libdynamo.so : dynamo.cpp dynamo.h dynamo-system.cpp dynamo-system.h
	$(CC) $(CFLAGS) -shared $(LDFLAGS) -o $@ $< $(LDLIBS)

test : main libdynamo.so
	./$<

clean :
	$(RM) *.o *.so main
