#ifndef ___FILE_BUFFER___
#define ___FILE_BUFFER___

#include <fstream>
#include <string>

using namespace std;

class FileBuffer {
	private:
		static const int BUFFER_SIZE = 1024 * 512; //kb
	
	public:
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