#ifndef ___OPTIONS___
#define ___OPTIONS___

#include <string>
#include <vector>

#include "leet.h"

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

		static const int DEFAULT_OPT_MAX_COMBINED_NUMS;
		static const bool DEFAULT_OPT_LEET;
		static const bool  DEFAULT_OPT_USE_PREPEND;
		static const string DEFAULT_OPT_PREPEND;
		static const bool  DEFAULT_OPT_USE_APPEND;
		static const string DEFAULT_OPT_APPEND;

		//Data
		bool dataIsOrganization = false;
		bool dataIsIndividual = false;
		string dataNames = "";
		string dataKeywords = "";
		string dataDates = "";
		string dataNumbers = "";
		
		//Keyspace
		int ksMin = DEFAULT_KS_MIN;
		int ksMax = DEFAULT_KS_MAX;
		bool ksUseUpper = DEFAULT_KS_USE_UPPER;
		bool ksUseLower = DEFAULT_KS_USE_LOWER;
		bool ksUseNum = DEFAULT_KS_USE_NUM;
		
		//Options
		int optMaxCombinedNums = DEFAULT_OPT_MAX_COMBINED_NUMS;
		bool optUseLeet = DEFAULT_OPT_LEET;
		bool optUsePrepend = DEFAULT_OPT_USE_PREPEND;
		string optPrepend = DEFAULT_OPT_PREPEND;
		bool optUseAppend = DEFAULT_OPT_USE_APPEND;
		string optAppend = DEFAULT_OPT_APPEND;
		
		vector<shared_ptr<Leet>> optLeets;
		
		Options(vector<shared_ptr<Leet>> optLeets);
		void demoOrganization();
		vector<string> getPrependSequences();
		vector<string> getPrependSequences(bool clearCache);
		vector<string> getAppendSequences();
		vector<string> getAppendSequences(bool clearCache);

		friend ostream& operator<<(ostream& os, const shared_ptr<Options> obj);

	private:
		vector<string> prependSequences;
		vector<string> appendSequences;
};

#endif