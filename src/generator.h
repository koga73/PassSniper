#ifndef ___GENERATOR___
#define ___GENERATOR___

#include <vector>
#include <string>
#include <regex>
#include <chrono>

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
		int ksMinOffset = 0;
		int totalWordCount = 0;
		int wordCount = 0;
		chrono::system_clock::time_point beginTime;
		chrono::system_clock::time_point lastFlush;
		
		void filter();
		void cases();
		void combine();
		void combine(const string& currentWord);
		vector<string> variations(const string& word);
		vector<string> leet(const string& word);
		vector<string> prepend(const string& word);
		vector<string> append(const string& word);
		void addLine(const string& line);
};

#endif