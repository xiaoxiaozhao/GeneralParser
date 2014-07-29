rm bin/GeneralParser
g++ -g -rdynamic -o bin/GeneralParser src/generalParser.cpp bin/lib/libgp.a -lpthread -ldl -lhiredis 
