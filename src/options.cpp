#include <iostream>
#include <vector>
#include <string>

#include "options.h"
#include "utils.h"

using namespace std;

const int Options::KS_MIN = 1;
const int Options::KS_MAX = 255;

const int Options::DEFAULT_KS_MIN = 7;
const int Options::DEFAULT_KS_MAX = 12;
const bool Options::DEFAULT_KS_USE_UPPER = true;
const bool Options::DEFAULT_KS_USE_LOWER = true;
const bool Options::DEFAULT_KS_USE_NUM = true;

const int Options::DEFAULT_OPT_MAX_COMBINED_NUMS = 4;
const bool Options::DEFAULT_OPT_LEET = true;
const bool Options::DEFAULT_OPT_USE_PREPEND = true;
const string Options::DEFAULT_OPT_PREPEND = "!,.,@,#,1";
const bool Options::DEFAULT_OPT_USE_APPEND = true;
const string Options::DEFAULT_OPT_APPEND = "!,.,@,#,1,123";

Options::Options(vector<Leet*> optLeets){
	this->optLeets = optLeets;
}

void Options::demoOrganization(){
	dataIsOrganization = true;
	dataIsIndividual = false;
	dataNames = "initech";
	dataKeywords = "tps,swingline";
	dataDates = "02-19-1999";
	dataNumbers = "4120,555-0157";
}

vector<string> Options::getPrependSequences(){
	return getPrependSequences(false);
}
vector<string> Options::getPrependSequences(bool clearCache){
	if (clearCache){
		prependSequences = Utils::split(optPrepend);
	}
	return prependSequences;
}

vector<string> Options::getAppendSequences(){
	return getAppendSequences(false);
}
vector<string> Options::getAppendSequences(bool clearCache){
	if (clearCache){
		appendSequences = Utils::split(optAppend);
	}
	return appendSequences;
}

ostream& operator<<(ostream& os, const Options* obj){
	//Data
	os << "dataIsOrganization: " << obj->dataIsOrganization << endl;
	os << "dataIsIndividual: " << obj->dataIsIndividual << endl;
	os << "dataNames: " << obj->dataNames << endl;
	os << "dataKeywords: " << obj->dataKeywords << endl;
	os << "dataDates: " << obj->dataDates << endl;
	os << "dataNumbers: " << obj->dataNumbers << endl;

	//Keyspace
	os << "ksMin: " << obj->ksMin << endl;
	os << "ksMax: " << obj->ksMax << endl;
	os << "ksUseLower: " << obj->ksUseLower << endl;
	os << "ksUseUpper: " << obj->ksUseUpper << endl;
	os << "ksUseNum: " << obj->ksUseNum << endl;
	
	//Options
	os << "optMaxCombinedNums: " << obj->optMaxCombinedNums << endl;
	os << "optUseLeet: " << obj->optUseLeet << endl;
	os << "optUsePrepend: " << obj->optUsePrepend << endl;
	os << "optPrepend: " << obj->optPrepend << endl;
	os << "optUseAppend: " << obj->optUseAppend << endl;
	os << "optAppend: " << obj->optAppend << endl;

	return os;
}