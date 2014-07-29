rm bin/simplerParser1.so
g++ -g -fPIC -shared -lpthread -lhiredis -o bin/simplerParser1.so src/simplerHandler1.cpp src/simplerParser.cpp ../bin/lib/libgp.a
