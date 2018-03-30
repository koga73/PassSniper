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
		void addLine(string line);
		void flush();
		void close();

	private:
		string fileName;
		bool isOpen;
		bool keepOpen;
		string lines;
		int linesLen;
		ofstream ofs;
};

#endif