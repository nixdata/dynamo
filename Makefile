CFLAGS  = -std=c++11 -pedantic -Wall -O2 -fPIC 
LDLIBS  = -ldl

all : server lib-dynomer.so

server : server.cpp dynomer.h 
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)

lib-dynomer.so : dynomer.cpp dynomer.h platform.h
	$(CC) $(CFLAGS) -shared $(LDFLAGS) -o $@ $< $(LDLIBS)

test : server lib-dynomer.so
	./$<

clean :
	$(RM) *.o *.so server
