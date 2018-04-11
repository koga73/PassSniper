#ifndef ___LEET___
#define ___LEET___

#include <iostream>
#include <vector>
#include <string>
#include <regex>

using namespace std;

class Leet {
	public:
		static const string CONFIG;
		
		static const regex REGEX_NEWLINE;
		static const regex REGEX_WHITESPACE;

		vector<string> from;
		vector<string> to;

		Leet(vector<string> from, vector<string> to);

		static vector<Leet*> parse();
		static vector<Leet*> parse(const string& configData);

		friend ostream& operator<<(ostream& os, const Leet* obj);
};

#endif