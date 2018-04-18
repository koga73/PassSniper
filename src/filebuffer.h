#ifndef ___FILE_BUFFER___
#define ___FILE_BUFFER___

#include <fstream>
#include <vector>
#include <string>
#include <pthread.h>

using namespace std;

class FileBuffer {
	private:
		static const unsigned long BUFFER_SIZE = 1024 * 1024; //kb
	
	public:
		FileBuffer(string fileName);
		FileBuffer(string fileName, bool keepOpen);
		~FileBuffer();
		bool test();
		bool addLine(const string& line);
		bool addLines(const vector<string>& lines);
		void flush();
		void close();

	private:
		pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
		string fileName = "";
		bool keepOpen = false;
		bool isOpen = false;
		string data;
		unsigned long dataLen = 0;
		ofstream ofs;
};

#endif