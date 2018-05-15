all: build64 build32

build64:
	g++ src/*.cpp -o bin/passsniper64 -static -lpthread -O3

build32:
	g++ -m32 src/*.cpp -o bin/passsniper32 -static -lpthread -O3

mac: build64-mac build32-mac

build64-mac:
	clang++ src/*.cpp -o bin/passsniper64.app -std=c++14 -stdlib=libc++ -pthread -O3

build32-mac:
	clang++ -arch i386 src/*.cpp -o bin/passsniper32.app -std=c++14 -stdlib=libc++ -pthread -O3
