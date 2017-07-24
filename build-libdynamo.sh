cc -std=c++11 -pedantic -Wall -Werror -O2 -fPIC -shared -o libdynamo.so dynamo.cpp dynamo-system.cpp -ldl
