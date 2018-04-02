#ifndef ___GENERATOR___
#define ___GENERATOR___

#include <vector>
#include <string>
#include <regex>

#include "filebuffer.h"
#include "options.h"

using namespace std;

class Generator {
	public:
		Generator(Options*& options, FileBuffer*& fb);
		void generate();
		
	private:
		static const regex REGEX_COMMA_WHITESPACE;
		static const regex REGEX_NON_NUMERIC;
		static const regex REGEX_NUMERIC;
		static const regex REGEX_LOWERCASE;
		static const regex REGEX_UPPERCASE;

		Options*& options;
		FileBuffer*& fb;
		vector<string> words;
		
		void filter();
		void cases();
		void combine();
		void combine(string currentWord);
		void leet();
		void prepend();
		void append();
};

#endif