#include <string>
#include <cstring>
#include <vector>
#include <regex>

#include "utils.h"

using namespace std;

vector<string> Utils::split(const string s){
	return split(s, ',');
}
vector<string> Utils::split(const string s, const char delim){
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
vector<string> Utils::split(const string s, const regex re){
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

bool Utils::isNumeric(const string& s){
    return !s.empty() && find_if(s.begin(), s.end(), [](char c){
        return !isdigit(c);
    }) == s.end();
}

string Utils::formatCommas(const string& s){
    string formatted = s;
    int insertPosition = formatted.length() - 3;
    while (insertPosition > 0) {
        formatted.insert(insertPosition, ",");
        insertPosition -= 3;
    }
    return formatted;
}