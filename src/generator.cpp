#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <algorithm>

#include "generator.h"
#include "utils.h"

using namespace std;

Generator::Generator(Options*& options, FileBuffer*& fb) : options(options), fb(fb) {}

void Generator::generate(){
	int i;
    regex regexCommaWhiteSpace = regex("[,\\s]");
    regex regexNonNumeric = regex("\\D");

    vector<string> names = Utils::split(options->dataNames, regexCommaWhiteSpace);
    vector<string> keywords = Utils::split(options->dataKeywords, regexCommaWhiteSpace);
    vector<string> dates = Utils::split(options->dataDates, regexNonNumeric);
    vector<string> numbers = Utils::split(options->dataNumbers, regexNonNumeric);

    //Smart dates
    //Create 2 digit year from 4
    //Strip out non-numerics
    //Create entry without leading 0s
    int datesLen = dates.size();
    for (i = 0; i < datesLen; i++){
        string snum = dates.at(i);
        if (snum.length() == 4){ //Create 2 digit year
            dates.push_back(snum.substr(2, 2));
        }
        int num = -1;
        try {
            num = stoi(snum);
        } catch (exception ex){
            dates.erase(dates.begin() + i);
            datesLen--;
            i--;
        }
        if (num == -1){
            continue;
        }
        string shortNum = to_string(num);
        if (shortNum != snum){ //Leading 0s
            dates.push_back(shortNum);
        }
    }

    Utils::concat(words, names);
    Utils::concat(words, keywords);
    Utils::concat(words, dates);
    Utils::concat(words, numbers);

    filter();

    int wordsLen = words.size();
    for (i = 0; i < wordsLen; i++){
        cout << words.at(i) << endl;
        fb->addLine(words.at(i));
    }
    fb->flush();
}

//Empty
//Too long
//Contains number if numeric not specified
//Duplicates
void Generator::filter(){
    regex regexNumeric = regex("\\d");

    int wordsLen = words.size();
    for (int i = 0; i < wordsLen; i++){
        bool shouldRemove = false;
        string word = words.at(i);
        transform(word.begin(), word.end(), word.begin(), ::tolower);
        if (!word.size() || word.size() > options->ksMax){
            shouldRemove = true;
        }
        if (!options->ksUseNum && regex_search(word, regexNumeric)){
            shouldRemove = true;
        }
        if (!shouldRemove){
            for (int j = i + 1; j < wordsLen; j++){
                string word2 = words.at(j);
                transform(word2.begin(), word2.end(), word2.begin(), ::tolower);
                if (word == word2){
                    shouldRemove = true;
                    break;
                }
            }
        }
        if (shouldRemove){
            words.erase(words.begin() + i);
            wordsLen--;
            i--;
        }
    }
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