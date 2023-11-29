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
