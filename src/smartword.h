#ifndef ___SMART_WORD___
#define ___SMART_WORD___

#include <string>

using namespace std;

class SmartWord {
	public:
		string word;
		string baseWord;
		bool isNumeric;

		SmartWord(const string& word, const string& baseWord, const bool isNumeric);

		//friend ostream& operator<<(ostream& os, const shared_ptr<SmartWord> obj);
};

#endif