#include <vector>
#include <string>
#include <regex>

#include "generator.h"
#include "utils.h"

using namespace std;

void Generator::generate(Options*& options, FileBuffer*& fb){
    vector<string> names = Utils::split(options->dataNames);
    vector<string> keywords = Utils::split(options->dataKeywords);
    vector<string> dates = Utils::split(options->dataDates);
    vector<string> numbers = Utils::split(options->dataNumbers);

    vector<string> smartDates;
    int datesLen = dates.size();
    for (int i = 0; i < datesLen; i++){
        vector<string> splitDates = Utils::split(dates.at(i), '-');
        Utils::concat(smartDates, splitDates);
    }
    //Create 2 digit year from 4
    //Strip out non-numerics
    //Create entry without leading 0s
    int smartDatesLen = smartDates.size();
    for (int i = 0; i < smartDatesLen; i++){
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
    for (int i = 0; i < numbersLen; i++){
        vector<string> splitNumbers = Utils::split(numbers.at(i), regex("\\D"));
        Utils::concat(smartNumbers, splitNumbers);
    }

    vector<string> combined;
    Utils::concat(combined, names);
    Utils::concat(combined, keywords);
    Utils::concat(combined, smartDates);
    Utils::concat(combined, smartNumbers);

    int combinedLen = combined.size();
    for (int i = 0; i < combinedLen; i++){
        fb->addLine(combined.at(i));
    }
    fb->flush();
}