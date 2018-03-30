#include <iostream>

#include "options.h"

using namespace std;

const int Options::DEFAULT_KS_MIN = 7;
const int Options::DEFAULT_KS_MAX = 15;
const bool Options::DEFAULT_KS_USE_UPPER = true;
const bool Options::DEFAULT_KS_USE_LOWER = true;
const bool Options::DEFAULT_KS_USE_NUM = true;

const bool Options::DEFAULT_OPT_LEET = true;
const bool Options::DEFAULT_OPT_USE_PREPEND = true;
const string Options::DEFAULT_OPT_PREPEND = "!,#,$,1,123";
const bool Options::DEFAULT_OPT_USE_APPEND = true;
const string Options::DEFAULT_OPT_APPEND = "!,#,$,1,123";

void Options::demoOrganization(){
	dataIsIndividual = false;
	dataIsOrganization = true;
	dataNames = "initech";
	dataKeywords = "tps,swingline";
	dataDates = "02-19-1999";
	dataNumbers = "4120";
}

ostream& operator<<(ostream& os, const Options* obj){
	os <<
	//Data
	"dataIsIndividual: " << obj->dataIsIndividual << endl <<
	"dataIsOrganization: " << obj->dataIsOrganization << endl <<
	"dataNames: " << obj->dataNames << endl <<
	"dataKeywords: " << obj->dataKeywords << endl <<
	"dataDates: " << obj->dataDates << endl <<
	"dataNumbers: " << obj->dataNumbers << endl <<

	//Keyspace
	"ksMin: " << obj->ksMin << endl <<
	"ksMax: " << obj->ksMax << endl <<
	"ksUseLower: " << obj->ksUseLower << endl <<
	"ksUseUpper: " << obj->ksUseUpper << endl <<
	"ksUseNum: " << obj->ksUseNum << endl <<
	
	//Options
	"optLeet: " << obj->optLeet << endl <<
	"optUsePrepend: " << obj->optUsePrepend << endl <<
	"optPrepend: " << obj->optPrepend << endl <<
	"optUseAppend: " << obj->optUseAppend << endl <<
	"optAppend: " << obj->optAppend << endl;

	return os;
}