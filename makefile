all:
	build64 build32

build64:
	g++ src/*.cpp -o bin/passsniper64 -static -lpthread -O3

build32:
	g++ src/*.cpp -o bin/passsniper32 -static -lpthread -O3