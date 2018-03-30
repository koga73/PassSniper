#include <string>
#include <cstring>
#include <vector>

#include "utils.h"

using namespace std;

vector<string>* Utils::split(const string csv){
	return split(csv, ',');
}
vector<string>* Utils::split(const string csv, const char delim){
	vector<string>* ret = new vector<string>();

	//to avoid modifying original string
    //first duplicate the original string and return a char pointer then free the memory
    char * dup = strdup(csv.c_str());
    char * token = strtok(dup, &delim);
    while(token != NULL){
        ret->push_back(string(token));
        //the call is treated as a subsequent calls to strtok:
        //the function continues from where it left in previous invocation
        token = strtok(NULL, &delim);
    }
    free(dup);

	return ret;
}