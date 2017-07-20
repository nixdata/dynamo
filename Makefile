CFLAGS  = -std=c++11 -pedantic -Wall -Werror -O2 -fPIC 
LDLIBS  = -ldl

all : server libdynomer.so

server : server.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)

libdynomer.so : dynomer.cpp dynomer-system.cpp
	$(CC) $(CFLAGS) -shared $(LDFLAGS) -o $@ $< $(LDLIBS)

test : server libdynomer.so
	./$<

clean :
	$(RM) *.o *.so server
