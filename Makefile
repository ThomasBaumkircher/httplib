all: init httplib.o http_tools.o cstrings.o
	clang -shared -o libhttp.so build/httplib.o build/http_tools.o build/cstrings.o

http_tools.o: src/http_tools.c
	clang -Wall -Werror -pedantic -fpic -Iinclude -c src/http_tools.c -o build/http_tools.o

httplib.o: src/httplib.c
	clang -Wall -Werror -pedantic -fpic -Iinclude -c src/httplib.c -o build/httplib.o

cstrings.o: src/cstrings.c
	clang -Wall -Werror -pedantic -fpic -Iinclude -c src/cstrings.c -o build/cstrings.o

init:
	mkdir -p build

clean:
	rm -rf build/*

test: all
	clang test/test.c -L. -lhttp -o bin/test -g
	./bin/test

test_static: all
	clang test/test.c -Iinclude build/httplib.o build/http_tools.o build/cstrings.o -o bin/test -g