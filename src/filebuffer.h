#ifndef ___FILE_BUFFER___
#define ___FILE_BUFFER___

#include <fstream>
#include <string>

using namespace std;

class FileBuffer {
	public:
		static const int MAX_LINES = 1024 * 512;
		
		FileBuffer(string fileName);
		FileBuffer(string fileName, bool keepOpen);
		bool test();
		void addLine(string line);
		void flush();
		void close();

	private:
		string fileName;
		bool keepOpen;
		string lines[MAX_LINES];
		int lineIndex = -1;
		bool isOpen;
		ofstream ofs;
};

#endif