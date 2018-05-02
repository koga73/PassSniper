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
		bool test();
		bool addLine(const string& line);
		bool addLines(const vector<string>& lines);
		void flush();

	private:
		pthread_mutex_t mutex;
		string fileName;
		string data;
		unsigned long dataLen;
		ofstream ofs;
};

#endif