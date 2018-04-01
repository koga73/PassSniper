#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <algorithm>

#include "generator.h"
#include "utils.h"

using namespace std;

const regex Generator::REGEX_COMMA_WHITESPACE = regex("[,\\s]");
const regex Generator::REGEX_NON_NUMERIC = regex("\\D");
const regex Generator::REGEX_NUMERIC = regex("\\d");
const regex Generator::REGEX_LOWERCASE = regex("[a-z]");
const regex Generator::REGEX_UPPERCASE = regex("[A-Z]");

Generator::Generator(Options*& options, FileBuffer*& fb) : options(options), fb(fb) {}

void Generator::generate(){
	int i;
    vector<string> names = Utils::split(options->dataNames, REGEX_COMMA_WHITESPACE);
    vector<string> keywords = Utils::split(options->dataKeywords, REGEX_COMMA_WHITESPACE);
    vector<string> dates = Utils::split(options->dataDates, REGEX_NON_NUMERIC);
    vector<string> numbers = Utils::split(options->dataNumbers, REGEX_NON_NUMERIC);

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
    cases();

    int wordsLen = words.size();
    for (i = 0; i < wordsLen; i++){
        cout << words.at(i) << endl;
        fb->addLine(words.at(i));
    }
    fb->flush();
}

//Empty
//Too long
//Contains number if not specified
//Duplicates
void Generator::filter(){
    int wordsLen = words.size();
    for (int i = 0; i < wordsLen; i++){
        bool shouldRemove = false;
        string word = words.at(i);
        transform(word.begin(), word.end(), word.begin(), ::tolower);

        //Empty / Too long
        if (!word.size() || word.size() > options->ksMax){
            shouldRemove = true;
        }
        //Contains number if not specified
        if (!shouldRemove && !options->ksUseNum && regex_search(word, REGEX_NUMERIC)){
            shouldRemove = true;
        }
        //Duplicates
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
    int wordsLen = words.size();
    for (int i = 0; i < wordsLen; i++){
        string word = words.at(i);

        //Check state of current word and remove if not supported
        bool shouldRemove = false;
        if (!options->ksUseLower && regex_search(word, REGEX_LOWERCASE)){
            shouldRemove = true;
        }
        if (!options->ksUseUpper && regex_search(word, REGEX_UPPERCASE)){
            shouldRemove = true;
        }
        if (shouldRemove){
            words.erase(words.begin() + i);
            wordsLen--;
            i--;
        }

        //Lowercase
        if (options->ksUseLower){
            string lowerCase = word;
            transform(lowerCase.begin(), lowerCase.end(), lowerCase.begin(), ::tolower);
            if (lowerCase != word){
                words.insert(words.begin() + i + 1, lowerCase);
                i++;
                wordsLen++;
            }
        }

        //Uppercase
        if (options->ksUseUpper){
            string upperCase = word;
            transform(upperCase.begin(), upperCase.end(), upperCase.begin(), ::toupper);
            if (upperCase != word){
                words.insert(words.begin() + i + 1, upperCase);
                i++;
                wordsLen++;
            }
        }

        //Sentence case
        if (options->ksUseLower && options->ksUseUpper){
            string sentenceCase = word;
            transform(sentenceCase.begin(), sentenceCase.end(), sentenceCase.begin(), ::tolower);
            string firstLetter = sentenceCase.substr(0, 1);
            transform(firstLetter.begin(), firstLetter.end(), firstLetter.begin(), ::toupper);
            sentenceCase = firstLetter + sentenceCase.substr(1, sentenceCase.length() - 1);
            if (sentenceCase != word){
                words.insert(words.begin() + i + 1, sentenceCase);
                i++;
                wordsLen++;
            }
        }
    }
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