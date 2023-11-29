all: init httplib.o http_tools.o strings.o
	gcc -shared -o libhttp.so build/httplib.o build/http_tools.o build/strings.o

	sudo cp libhttp.so /usr/lib
	sudo cp include/*.h /usr/include
	sudo ldconfig

http_tools.o:
	gcc -Wall -Werror -pedantic -fpic -c src/http_tools.c -o build/http_tools.o

httplib.o:
	gcc -Wall -Werror -pedantic -fpic -c src/httplib.c -o build/httplib.o

strings.o:
	gcc -Wall -Werror -pedantic -fpic -c src/strings.c -o build/strings.o

init:
	mkdir -p build

clean:
	rm -rf build/*