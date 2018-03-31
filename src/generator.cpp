#include <iostream>
#include <vector>
#include <string>
#include <regex>

#include "generator.h"
#include "utils.h"

using namespace std;

Generator::Generator(Options*& options, FileBuffer*& fb) : options(options), fb(fb) {}

void Generator::generate(){
	int i;
    vector<string> names = Utils::split(options->dataNames);
    vector<string> keywords = Utils::split(options->dataKeywords);
    vector<string> dates = Utils::split(options->dataDates);
    vector<string> numbers = Utils::split(options->dataNumbers);

    vector<string> smartDates;
    int datesLen = dates.size();
    for (i = 0; i < datesLen; i++){
        vector<string> splitDates = Utils::split(dates.at(i), '-');
        Utils::concat(smartDates, splitDates);
    }
    //Create 2 digit year from 4
    //Strip out non-numerics
    //Create entry without leading 0s
    int smartDatesLen = smartDates.size();
    for (i = 0; i < smartDatesLen; i++){
        string snum = smartDates.at(i);
        if (snum.length() == 4){ //Create 2 digit year
            smartDates.push_back(snum.substr(2, 2));
        }
        int num = -1;
        try {
            num = stoi(snum);
        } catch (exception ex){
            smartDates.erase(smartDates.begin() + i);
            smartDatesLen--;
            i--;
        }
        if (num == -1){
            continue;
        }
        string shortNum = to_string(num);
        if (shortNum != snum){ //Leading 0s
            smartDates.push_back(shortNum);
        }
    }

    vector<string> smartNumbers;
    int numbersLen = numbers.size();
    for (i = 0; i < numbersLen; i++){
        vector<string> splitNumbers = Utils::split(numbers.at(i), regex("\\D"));
        Utils::concat(smartNumbers, splitNumbers);
    }

    Utils::concat(words, names);
    Utils::concat(words, keywords);
    Utils::concat(words, smartDates);
    Utils::concat(words, smartNumbers);

    int wordsLen = words.size();
    for (i = 0; i < wordsLen; i++){
        fb->addLine(words.at(i));
    }
    fb->flush();
}

//Filter duplicates and too long
void Generator::filter(){

}

//Build case variations
void Generator::cases(){

}

//Combine words to fill keyspace
void Generator::combine(){

}

//Leet substitution
void Generator::leet(){

}

//Prepend sequences
void Generator::prepend(){

}

//Append sequences
void Generator::append(){

}