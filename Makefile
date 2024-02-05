all:
	gcc -c src/*.c

	cp *.o ~/source/SYT-Mo/02/server/lib
	cp include/* ~/source/SYT-Mo/02/server/include
	cp *.o ~/source/SYT-Mo/02/client/lib
	cp include/* ~/source/SYT-Mo/02/client/include

	sudo cp *.o /usr/lib/httplib
	sudo cp include/* /usr/include/httplib

test: all
	gcc -o bin/test test/test.c *.o
	./bin/test

init:
	mkdir -p build
	mkdir -p bin

clean:
	rm build/*
	rm bin/*

all: init httplib.o http_tools.o cstrings.o
	gcc -shared -o libhttp.so build/httplib.o build/http_tools.o build/cstrings.o

	sudo cp libhttp.so /usr/lib
	sudo cp include/*.h /usr/include
	sudo ldconfig

http_tools.o:
	gcc -Wall -Werror -pedantic -fpic -Iinclude -c src/http_tools.c -o build/http_tools.o

httplib.o:
	gcc -Wall -Werror -pedantic -fpic -Iinclude -c src/httplib.c -o build/httplib.o

cstrings.o:
	gcc -Wall -Werror -pedantic -fpic -Iinclude -c src/cstrings.c -o build/cstrings.o

init:
	mkdir -p build

clean:
	rm -rf build/*
>>>>>>> 189e4e51b651e103dbf33ac9ca700f1f1962ee5e
