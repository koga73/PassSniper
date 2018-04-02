#ifndef ___FILE_BUFFER___
#define ___FILE_BUFFER___

#include <fstream>
#include <string>

using namespace std;

class FileBuffer {
	public:
		static const int BUFFER_SIZE = 1024 * 100; //kb
		
		FileBuffer(string fileName);
		FileBuffer(string fileName, bool keepOpen);
		bool test();
		bool addLine(string line);
		void flush();
		void close();

	private:
		string fileName = "";
		bool keepOpen = false;
		bool isOpen = false;
		string lines = "";
		int linesLen = 0;
		ofstream ofs;
};

#endif