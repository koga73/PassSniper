#ifndef ___GENERATOR___
#define ___GENERATOR___

#include <vector>
#include <string>

#include "filebuffer.h"
#include "options.h"

using namespace std;

class Generator {
	public:
		Generator(Options*& options, FileBuffer*& fb);
		void generate();
		
	private:
		Options*& options;
		FileBuffer*& fb;
		vector<string> words;
		
		void filter();
		void cases();
		void combine();
		void leet();
		void prepend();
		void append();
};

#endif