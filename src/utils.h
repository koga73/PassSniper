#ifndef ___UTILS___
#define ___UTILS___

#include <string>
#include <vector>

using namespace std;

class Utils {
	public:
		static vector<string>* split(const string s);
		static vector<string>* split(const string s, const char delim);
};

#endif