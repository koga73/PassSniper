#include <iostream>
#include <vector>
#include <string>

#include "leet.h"
#include "utils.h"

using namespace std;

//TODO: Move to embeded resource / external file
const string Leet::CONFIG = 
	"#PassSniper leet substitution config" "\n"
	"# " "\n"
	"#Format: {CSV_SEARCH_CHARS}[SPACE]{CSV_REPLACE_SEQ}" "\n"
	"#Example: 'e,E 3' will replace 'e' and 'E' with '3'" "\n"
	"# " "\n"
	"#Order matters! Most common replacements should be at the top" "\n"
	"#Example input: 'moneys' will generate the following output:" "\n"
	"# mon3ys" "\n"
	"# m0n3ys" "\n"
	"# m0n3y5" "\n"
	"# m0n3y$" "\n"
	"# " "\n"
    "e,E 3" "\n"
    "o,O 0" "\n"
    "i,I 1" "\n"
    "a,A @,4" "\n"
    "s,S $,5" "\n" 
    "1 !" "\n"
    "l 1" "\n"
    "L 7" "\n"
    "h #" "\n"
    "z 2" "\n"
    "b,B 8" "\n"
    "c ("
;
const regex Leet::REGEX_COMMENT = regex("#.+?\n");
const regex Leet::REGEX_WHITESPACE = regex("\\s+");

Leet::Leet(vector<string> from, vector<string> to){
	this->from = from;
	this->to = to;
}

vector<Leet*> Leet::parse(){
	return parse(CONFIG);
}
vector<Leet*> Leet::parse(const string& config){
	vector<Leet*> leets;

	string configData = regex_replace(config, REGEX_COMMENT, "");
	vector<string> groups = Utils::split(configData, REGEX_WHITESPACE);
	int groupsLen = groups.size();
	if (groupsLen % 2){
		throw "Invalid Leet Config.";
	}
	for (int i = 0; i < groupsLen; i+=2){
		vector<string> from = Utils::split(groups.at(i));
		vector<string> to = Utils::split(groups.at(i + 1));
		leets.push_back(new Leet(from, to));
	}
	
	return leets;
}

ostream& operator<<(ostream& os, const Leet* obj){
	int i;

	int fromLen = obj->from.size();
	for (i = 0; i < fromLen; i++){
		os << obj->from.at(i);
		if (i != fromLen - 1){
			os << ',';
		}
	}
	os << " ";

	int toLen = obj->to.size();
	for (i = 0; i < toLen; i++){
		os << obj->to.at(i);
		if (i != toLen - 1){
			os << ',';
		}
	}

	return os;
}