#ifndef ___OPTIONS___
#define ___OPTIONS___

#include <iostream>

using namespace std;

class Options {
	public:
		//Data
		bool dataIsIndividual;
		bool dataIsOrganization;
		string dataNames;
		string dataKeywords;
		string dataDates;
		string dataNumbers;

		//Keyspace
		int ksMin;
		int ksMax;
		bool ksUseUpper;
		bool ksUseLower;
		bool ksUseNum;
		
		//Options
		bool optLeet;
		string optPrepend;
		string optAppend;

		friend ostream& operator<<(ostream& os, const Options* obj);
};


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
	"optPrepend: " << obj->optPrepend << endl <<
	"optAppend: " << obj->optAppend << endl;

	return os;
}

#endif