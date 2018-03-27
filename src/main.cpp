#include <iostream>
#include <conio.h>

#include "options.h"

using namespace std;

Options* getOptions();
int getNum(int defaultVal, string message);
bool getBool(bool defaultVal, string message);
string getString(string defaultVal, string message);
string getString();

int main(int argc, char **argv){
    cout << "    ____                  _____       _                " << endl;
    cout << "   / __ \\____ ___________/ ___/____  (_)___  ___  _____" << endl;
    cout << "  / /_/ / __ `/ ___/ ___/\\__ \\/ __ \\/ / __ \\/ _ \\/ ___/" << endl;
    cout << " / ____/ /_/ (__  |__  )___/ / / / / / /_/ /  __/ /    " << endl;
    cout << "/_/    \\__,_/____/____//____/_/ /_/_/ .___/\\___/_/     " << endl;
    cout << "                                   /_/                 " << endl;
    cout << "v1.0.0 - AJ Savino" << endl;
    cout << endl;
    cout << "Generating better quality wordlists for pentesting" << endl;
    cout << endl;
    cout << endl;
    cout << "Think before using... don't be a dick." << endl;
    char key;
    do {
        cout << "Press [ENTER] to assert that you are not being a dick or [ESCAPE] to exit" << endl;
        key = getch();
        if (key == 27){
            exit(0);
        }
    } while (key != 13);
    
    Options* options = getOptions();
    cout << endl << options << endl;

    system("PAUSE");
}

Options* getOptions(){
    Options* options = new Options();

    //Target
    bool isIndividual;
    bool isOrganization;
    do {
        string target;
        cout << endl;
        cout << "Select wordlist generation target:" << endl;
        cout << "  [1] Invididual" << endl;
        cout << "  [2] Organization" << endl;
        cout << ":";
        cin.clear();
        cin.sync();
        getline(cin, target);

        isIndividual = target == "1";
        isOrganization = target == "2";
        if (isIndividual || isOrganization){
            break;
        } else {
            cout << endl << "Please type [1] or [2] and press [ENTER]" << endl;
        }
    } while (true);
    options->dataIsIndividual = isIndividual;
    options->dataIsOrganization = isOrganization;

    //Names
    cout << endl;
    cout << "Names (Optional)(Comma Separated)" << endl;
    if (isIndividual){
        cout << "      (Pets,Children,Spouse,Other)" << endl;
    }
    if (isOrganization){
        cout << "      (Company,Street,Other)" << endl;
    }
    options->dataNames = getString();

    //Keywords
    cout << endl;
    cout << "Keywords (Optional)(Comma Separated)" << endl;
    if (isIndividual){
        cout << "         (Sports,Teams,Animals,Interests,Other)" << endl;
    }
    if (isOrganization){
        cout << "         (Mascot,Products,Other)" << endl;
    }
    options->dataKeywords = getString();

    //Dates
    cout << endl;
    cout << "Significant Dates (Optional)(Comma Separated)(mm-dd-yyyy)" << endl;
    if (isIndividual){
        cout << "                  (Anniversary,Birthdays,Other)" << endl;
    }
    if (isOrganization){
        cout << "                  (Founded,Other)" << endl;
    }
    options->dataDates = getString();

    //Numbers
    cout << endl;
    cout << "Significant Numbers (Optional)(Comma Separated)(555-5555)" << endl;
    if (isIndividual){
        cout << "                    (Favorite,Street,Phone,Other)" << endl;
    }
    if (isOrganization){
        cout << "                    (Street,Phone,ID,Other)" << endl;
    }
    options->dataNumbers = getString();

    options->ksMin = getNum(7, "Keyspace - Minimum Length");
    options->ksMax = getNum(15, "Keyspace - Maximum Length");
    options->ksUseLower = getBool(true, "Keyspace - Lowercase");
    options->ksUseUpper = getBool(true, "Keyspace - Uppercase");
    options->ksUseNum = getBool(true, "Keyspace - Numbers");
    options->optLeet = getBool(true, "Option - Leet Substitution (Example 'e' > '3')");
    options->optPrepend = getString("!,#,$,1,123", "Option - Prepend Sequences (Comma Separated)");
    options->optAppend = getString("!,#,$,1,123", "Option - Append Sequences (Comma Separated)");

    return options;
}

int getNum(int defaultVal, string message){
    int val = defaultVal;
    do {
        string input;
        cout << endl << message << endl;
        cout << ": (" << val << ") ";
        cin.clear();
        cin.sync();
        getline(cin, input);
        if (!input.length()){ //Default
            break;
        }
        try {
            val = stoi(input);
            break;
        } catch (exception ex){
            cout << endl << "Please type a valid number" << endl;
        }
    } while (true);
    return val;
}

bool getBool(bool defaultVal, string message){
    bool val = defaultVal;
    do {
        string input;
        cout << endl << message << endl;
        cout << ": (" << ((val) ? "y" : "n") << ") ";
        cin.clear();
        cin.sync();
        getline(cin, input);
        if (!input.length()){ //Default
            break;
        }
        if (input == "y" || input == "n"){
            val = input == "y";
            break;
        } else {
            cout << endl << "Please type 'y' or 'n'" << endl;
        }
    } while (true);
    return val;
}

string getString(){
    return getString("", "");
}
string getString(string defaultVal, string message){
    string val = defaultVal;
    string input;
    if (message.length()){
        cout << endl << message << endl;
    }
    cout << ":";
    if (val.length()){
        cout << " (" << val << ") ";
    }
    cin.clear();
    cin.sync();
    getline(cin, input);
    if (input.length()){
        val = input;
    }
    return val;
}