rm bin/simpleParser1.so
flex src/simpleScanner1.l
bison -d src/simpleParser1.y
mv *.c src/
mv *.h src/
g++ -g -fPIC -shared -lpthread -lhiredis -o bin/simpleParser1.so src/simpleHandler1.cpp src/lex.yy.c src/simpleParser1.tab.c ../bin/lib/libgp.a
rm src/*.c
rm src/*.h
