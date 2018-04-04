#include <string>
#include <cstring>
#include <vector>
#include <regex>
#include <algorithm>
#include <random>
#include <chrono>

#include "utils.h"

using namespace std;

vector<string> Utils::split(const string& s){
	return split(s, ',');
}
vector<string> Utils::split(const string& s, const char& delim){
	vector<string> ret;

	//to avoid modifying original string
    //first duplicate the original string and return a char pointer then free the memory
    char * dup = strdup(s.c_str());
    char * token = strtok(dup, &delim);
    while(token != NULL){
        ret.push_back(string(token));
        //the call is treated as a subsequent calls to strtok:
        //the function continues from where it left in previous invocation
        token = strtok(NULL, &delim);
    }
    free(dup);

	return ret;
}
vector<string> Utils::split(const string& s, const regex& re){
    vector<string> container = {
        sregex_token_iterator(s.begin(), s.end(), re, -1),
        sregex_token_iterator()
    };
    return container;
}

void Utils::concat(vector<string>& vecA, vector<string>& vecB){
    vecA.reserve(vecA.size() + vecB.size());
    vecA.insert(vecA.end(), vecB.begin(), vecB.end());
}

void Utils::randomize(vector<string>& vec){
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    shuffle(vec.begin(), vec.end(), default_random_engine(seed));
}

bool Utils::isNumeric(const string& s){
    return !s.empty() && find_if(s.begin(), s.end(), [](char c){
        return !isdigit(c);
    }) == s.end();
}

string Utils::formatCommas(int val){
    string formatted = to_string(val);
    int insertPosition = formatted.length() - 3;
    while (insertPosition > 0) {
        formatted.insert(insertPosition, ",");
        insertPosition -= 3;
    }
    return formatted;
}

string Utils::formatTime(double seconds){
    int hours = seconds / 60 / 60;
    seconds -= hours * 60 * 60;
    int minutes = seconds / 60;
    int wholeSeconds = (int)seconds % 60;
    float fractionSeconds = seconds - (int)seconds;
    string sseconds = to_string(wholeSeconds + fractionSeconds);
    if (to_string(wholeSeconds).length() == 1){
        sseconds = "0" + sseconds;
    }
    return to_string(hours) + ":" + to_string(minutes) + ":" + sseconds;
}

string Utils::replaceAll(const string& s, const string& from, const string& to){
    string replaced = s;
    if(!from.empty()){
        size_t start_pos = 0;
        while((start_pos = replaced.find(from, start_pos)) != string::npos) {
            replaced.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
    }
    return replaced;
}