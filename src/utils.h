#ifndef ___UTILS___
#define ___UTILS___

#include <string>
#include <vector>

using namespace std;

class Utils {
	public:
		static vector<string> split(const string s);
		static vector<string> split(const string s, const char delim);
		static vector<string> split(const string s, const regex re);
		static void concat(vector<string>& vecA, vector<string>& vecB);
		static bool is_number(const string& s);
};

#endif