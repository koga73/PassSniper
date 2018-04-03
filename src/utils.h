#ifndef ___UTILS___
#define ___UTILS___

#include <string>
#include <vector>

using namespace std;

class Utils {
	public:
		static vector<string> split(const string& s);
		static vector<string> split(const string& s, const char& delim);
		static vector<string> split(const string& s, const regex& re);
		static void concat(vector<string>& vecA, vector<string>& vecB);
		static void randomize(vector<string>& vec);
		static bool isNumeric(const string& s);
		static string formatCommas(int val);
		static string formatTime(double seconds);
};

#endif