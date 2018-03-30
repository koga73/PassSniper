#include <iostream>
#include <fstream>
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
    cout << "Generating better quality wordlists for pentesting" << endl;
    cout << endl;
    cout << "PassSniper v1.0.0 - AJ Savino" << endl;
    cout << "CLI Usage: passsniper [outputfile]" << endl;
    cout << endl;
    cout << endl;

    //Warning
    cout << "Think before using... don't be a dick." << endl;
    char key;
    do {
        cout << "Press [ENTER] to assert that you are not being a dick or [ESCAPE] to exit" << endl;
        key = getch();
        if (key == 27){
            exit(0);
        }
    } while (key != 13);

    //Verify output file
    string outputFile = "./output.txt";
    if (argc == 2){
        outputFile = argv[1];
    }
    outputFile = getString(outputFile, "Output File");
    ofstream ofs;
    try {
        if (!outputFile.length()){
            throw "Undefined output file.";
        }
        ofs.open(outputFile, ios::out | ios::app); //Open file for output and append to eof
        if (!ofs || !ofs.is_open()){
            throw "There was an error opening the output file.";
        }
        ofs.close();
    } catch (const char* ex){
        cout << ex << " Exiting." << endl;

        system("PAUSE");
        return 1;
    }
    
    //Get user options
    Options* options = getOptions();
    cout << endl << options << endl;

    system("PAUSE");
    return 0;
}

Options* getOptions(){
    Options* options = new Options();

    //Target
    bool isDemo;
    bool isIndividual;
    bool isOrganization;
    do {
        string target;
        cout << endl;
        cout << "Select wordlist generation target:" << endl;
        cout << "  [0] Demo - Organization" << endl;
        cout << "  [1] Invididual" << endl;
        cout << "  [2] Organization" << endl;
        cout << ":";
        cin.clear();
        cin.sync();
        getline(cin, target);

        isDemo = target == "0";
        isIndividual = target == "1";
        isOrganization = target == "2";
        if (isDemo || isIndividual || isOrganization){
            break;
        } else {
            cout << endl << "Please type '0', '1' or '2' and press [ENTER]" << endl;
        }
    } while (true);
    if (isDemo){
        options->demoOrganization();
        return options;
    }
    options->dataIsIndividual = isIndividual;
    options->dataIsOrganization = isOrganization;

    //Names
    cout << endl;
    cout << "Names (Optional)(Comma Separated)" << endl;
    if (options->dataIsIndividual){
        cout << "      (Pets,Children,Spouse,Other)" << endl;
    }
    if (options->dataIsOrganization){
        cout << "      (Company,Street,Other)" << endl;
    }
    options->dataNames = getString();

    //Keywords
    cout << endl;
    cout << "Keywords (Optional)(Comma Separated)" << endl;
    if (options->dataIsIndividual){
        cout << "         (Sports,Teams,Animals,Interests,Other)" << endl;
    }
    if (options->dataIsOrganization){
        cout << "         (Mascot,Products,Other)" << endl;
    }
    options->dataKeywords = getString();

    //Dates
    cout << endl;
    cout << "Significant Dates (Optional)(Comma Separated)(mm-dd-yyyy)" << endl;
    if (options->dataIsIndividual){
        cout << "                  (Anniversary,Birthdays,Other)" << endl;
    }
    if (options->dataIsOrganization){
        cout << "                  (Founded,Other)" << endl;
    }
    options->dataDates = getString();

    //Numbers
    cout << endl;
    cout << "Significant Numbers (Optional)(Comma Separated)(555-5555)" << endl;
    if (options->dataIsIndividual){
        cout << "                    (Favorite,Street,Phone,Other)" << endl;
    }
    if (options->dataIsOrganization){
        cout << "                    (Street,Phone,ID,Other)" << endl;
    }
    options->dataNumbers = getString();

    options->ksMin = getNum(Options::DEFAULT_KS_MIN, "Keyspace - Minimum Length");
    options->ksMax = getNum(Options::DEFAULT_KS_MAX, "Keyspace - Maximum Length");
    options->ksUseLower = getBool(Options::DEFAULT_KS_USE_LOWER, "Keyspace - Lowercase");
    options->ksUseUpper = getBool(Options::DEFAULT_KS_USE_UPPER, "Keyspace - Uppercase");
    options->ksUseNum = getBool(Options::DEFAULT_KS_USE_NUM, "Keyspace - Numbers");
    options->optLeet = getBool(Options::DEFAULT_OPT_LEET, "Option - Leet Substitution (Example 'e' > '3')");
    options->optUsePrepend = getBool(Options::DEFAULT_OPT_USE_PREPEND, "Option - Prepend sequences (Example 'password' > '123password')");
    if (options->optUsePrepend){
        options->optPrepend = getString(Options::DEFAULT_OPT_PREPEND, "Option - Prepend Sequences (Comma Separated)");
    }
    options->optUseAppend = getBool(Options::DEFAULT_OPT_USE_APPEND, "Option - Append sequences (Example 'password' > 'password123')");
    if (options->optUseAppend){
        options->optAppend = getString(Options::DEFAULT_OPT_APPEND, "Option - Append Sequences (Comma Separated)");
    }

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