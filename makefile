linux: build64-linux build32-linux

build64-linux:
	g++ src/*.cpp -o bin/passsniper64 -pthread -O3

build32-linux:
	g++ -m32 src/*.cpp -o bin/passsniper32 -pthread -O3

mac: build64-mac build32-mac

build64-mac:
	clang++ src/*.cpp -o bin/passsniper64.app -std=c++14 -stdlib=libc++ -pthread -O3

build32-mac:
	clang++ -arch i386 src/*.cpp -o bin/passsniper32.app -std=c++14 -stdlib=libc++ -pthread -O3
