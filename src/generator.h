#ifndef ___GENERATOR___
#define ___GENERATOR___

#include <vector>
#include <string>
#include <regex>
#include <chrono>
#include <pthread.h>
#include <unordered_map>

#include "filebuffer.h"
#include "options.h"
#include "smartword.h"

using namespace std;

class Generator {
	public:
		static const int DEFAULT_NUM_THREADS;

	private:
		static const float MSG_TIME_MIN;

		static const regex REGEX_COMMA_WHITESPACE;
		static const regex REGEX_NON_NUMERIC;
		static const regex REGEX_NUMERIC;
		static const regex REGEX_LOWERCASE;
		static const regex REGEX_UPPERCASE;
		
	public:
		Generator(shared_ptr<Options> options, shared_ptr<FileBuffer> fb);
		~Generator();
		void generate();
		void generate(const int numThreads);
		void threadCombine();
		
	private:
		shared_ptr<Options> options;
		shared_ptr<FileBuffer> fb;
		vector<shared_ptr<SmartWord>> smartWords;
		int ksMinOffset = 0;
		int prependSequencesLen = 0;
		int appendSequencesLen = 0;
		unsigned int wordCount = 0;
		unsigned long totalWordCount = 0;
		chrono::system_clock::time_point beginTime;
		chrono::system_clock::time_point lastFlush;

		//Threading
		pthread_mutex_t combineMutex;
		pthread_mutex_t addLineMutex;
		int threadWordIndex = 0;
		friend void* threadCombineEntry(void*);
		
		void filter(vector<string>& words);
		void cases(vector<string>& words);
		void buildSmartWords(vector<string>& words);
		void combine(const int numThreads);
		void combine();
		void combine(const string& currentWord, unordered_map<string, unsigned char> wordCounts);
		void addVariations(const string& word);
		void addVariations(const string& word, const int splitIndex);
		void leet(vector<string>& words, const string& word);
		string reduceDuplicate(const string& word, const int splitIndex);
		void prepend(vector<string>& words, const string& word);
		void append(vector<string>& words, const string& word);
		void addLine(const string& line);
		void addLines(const vector<string>& lines);
};

#endif