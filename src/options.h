#ifndef ___OPTIONS___
#define ___OPTIONS___

using namespace std;

class Options {
	public:
		static const int KS_MIN;
		static const int KS_MAX;

		static const int DEFAULT_KS_MIN;
		static const int DEFAULT_KS_MAX;
		static const bool DEFAULT_KS_USE_UPPER;
		static const bool DEFAULT_KS_USE_LOWER;
		static const bool DEFAULT_KS_USE_NUM;

		static const bool DEFAULT_OPT_LEET;
		static const bool  DEFAULT_OPT_USE_PREPEND;
		static const string DEFAULT_OPT_PREPEND;
		static const bool  DEFAULT_OPT_USE_APPEND;
		static const string DEFAULT_OPT_APPEND;

		//Data
		bool dataIsIndividual;
		bool dataIsOrganization;
		string dataNames;
		string dataKeywords;
		string dataDates;
		string dataNumbers;
		
		//Keyspace
		int ksMin = DEFAULT_KS_MIN;
		int ksMax = DEFAULT_KS_MAX;
		bool ksUseUpper = DEFAULT_KS_USE_UPPER;
		bool ksUseLower = DEFAULT_KS_USE_LOWER;
		bool ksUseNum = DEFAULT_KS_USE_NUM;
		
		//Options
		bool optLeet = DEFAULT_OPT_LEET;
		bool optUsePrepend = DEFAULT_OPT_USE_PREPEND;
		string optPrepend = DEFAULT_OPT_PREPEND;
		bool optUseAppend = DEFAULT_OPT_USE_APPEND;
		string optAppend = DEFAULT_OPT_APPEND;

		void demoOrganization();

		friend ostream& operator<<(ostream& os, const Options* obj);
};

#endif