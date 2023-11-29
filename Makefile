all:
	gcc -c -Wall -Werror -pedantic -fpic src/*.c
	gcc -shared -o libhttp.so *.o
	mv *.o build

	sudo cp libhttp.so /usr/lib
	sudo cp include/* /usr/include
	sudo ldconfig

test: all
	gcc -o bin/test test/test.c -lhttp
	./bin/test

init:
	mkdir -p build
	mkdir -p bin

clean:
	rm -rf build/*
	rm -rf bin/*