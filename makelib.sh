rm bin/lib/*.o
rm bin/lib/libgp.a
g++ -fPIC -g -c src/config.cpp src/dataHelper.cpp src/dynamicLinker.cpp src/logger.cpp src/redisManager.cpp src/utfconv.cpp src/yyInput.cpp
mv *.o bin/lib/
ar -rsv bin/lib/libgp.a bin/lib/config.o bin/lib/dataHelper.o bin/lib/dynamicLinker.o bin/lib/logger.o bin/lib/redisManager.o bin/lib/utfconv.o bin/lib/yyInput.o
##this is a lib makefile
