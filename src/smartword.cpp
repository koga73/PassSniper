#include <iostream>
#include <string>

#include "smartword.h"

using namespace std;

SmartWord::SmartWord(const string& word, const string& baseWord, const bool isNumeric){
    this->word = word;
    this->baseWord = baseWord;
    this->isNumeric = isNumeric;
}

/*ostream& operator<<(ostream& os, const shared_ptr<SmartWord> obj){
	os << "word: " << obj->word << endl;
	os << "baseWord: " << obj->baseWord << endl;
	os << "isNumeric: " << obj->isNumeric << endl;

	return os;
}*/