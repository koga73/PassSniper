#ifndef ___UTILS___
#define ___UTILS___

#include <string>
#include <vector>

using namespace std;

class Utils {
	public:
		static vector<string>* split(const string csv);
		static vector<string>* split(const string csv, const char delim);
};

#endif