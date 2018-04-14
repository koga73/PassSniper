#include <string>
#include <cstring>
#include <vector>
#include <regex>
#include <algorithm>
#include <random>
#include <chrono>

#include "utils.h"

using namespace std;

const regex Utils::REGEX_COMMA = regex(",");

vector<string> Utils::split(const string& s){
	return split(s, REGEX_COMMA);
}
vector<string> Utils::split(const string& s, const regex& re){
    return {
        sregex_token_iterator(s.begin(), s.end(), re, -1),
        sregex_token_iterator()
    };
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
    while (insertPosition > 0){
        formatted.insert(insertPosition, ",");
        insertPosition -= 3;
    }
    return formatted;
}

string Utils::formatTime(double seconds){
    int hours = seconds / 3600;
    seconds = fmod(seconds, 3600);
    int minutes = seconds / 60;
    string sminutes = to_string(minutes);
    if (sminutes.length() == 1){
        sminutes = "0" + sminutes;
    }
    string sseconds = to_string(fmod(seconds, 60));
    if (to_string((int)seconds % 60).length() == 1){
        sseconds = "0" + sseconds;
    }
    return to_string(hours) + ":" + sminutes + ":" + sseconds;
}

void Utils::replaceAll(string& s, const string& from, const string& to){
    if(!from.empty()){
        size_t start_pos = 0;
        while((start_pos = s.find(from, start_pos)) != string::npos) {
            s.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
    }
}