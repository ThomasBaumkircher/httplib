all: init httplib.o http_tools.o cstrings.o
	gcc -shared -o libhttp.so build/httplib.o build/http_tools.o build/cstrings.o

http_tools.o: src/http_tools.c
	gcc -Wall -Werror -pedantic -fpic -Iinclude -c src/http_tools.c -o build/http_tools.o

httplib.o: src/httplib.c
	gcc -Wall -Werror -pedantic -fpic -Iinclude -c src/httplib.c -o build/httplib.o

cstrings.o: src/cstrings.c
	gcc -Wall -Werror -pedantic -fpic -Iinclude -c src/cstrings.c -o build/cstrings.o

init:
	mkdir -p build

clean:
	rm -rf build/*
