#include <iostream>
#include <conio.h>

#include "leet.h"
#include "options.h"
#include "filebuffer.h"
#include "generator.h"

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
    cout << "Generating targeted wordlists for pentesting" << endl;
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

    //Get output file
    string outputFile = "./output.txt";
    if (argc == 2){
        outputFile = argv[1];
    }
    outputFile = getString(outputFile, "Output File");

    FileBuffer* fb = new FileBuffer(outputFile);
    Options* options;
    Generator* gen;

    string errorMsg;
    try {
        //Test file access
        if (!fb->test()){
            throw "Could not open output file.";
        }

        //Get user options
        options = getOptions();
        //DEBUG
        cout << endl << options << endl;
        
        //Generate
        gen = new Generator(options, fb);
        gen->generate();
    } catch (ios::failure &ex){
        errorMsg = ex.what();
    } catch (const char* ex){
        errorMsg = string(ex);
    } catch (string ex){
        errorMsg = ex;
    } catch (...){
        errorMsg = "An error occured.";
    }
    if (errorMsg.length()){
        cout << errorMsg << " Exiting." << endl;
        cout << endl;
        system("PAUSE");
        return 1;
    }
    
    cout << endl;
    system("PAUSE");
    return 0;
}

Options* getOptions(){
    //TODO: Make Leet::parse() take in file parameter
    Options* options = new Options(Leet::parse());

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

    cout << endl << "LESS is MORE! Filling in everything will generate MILLIONS of combinations" << endl;

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
        cout << "                  (Anniversary,Birthdays,Graduation,Other)" << endl;
    }
    if (options->dataIsOrganization){
        cout << "                  (Founded,Other)" << endl;
    }
    options->dataDates = getString();

    //Numbers
    cout << endl;
    cout << "Significant Numbers (Optional)(Comma Separated)(555-0157)" << endl;
    if (options->dataIsIndividual){
        cout << "                    (Favorite,Street,Phone,Other)" << endl;
    }
    if (options->dataIsOrganization){
        cout << "                    (Street,Phone,ID,Other)" << endl;
    }
    options->dataNumbers = getString();

    cout << endl << "Keyspace and Options... if you're unsure just press enter to use the default value" << endl;

    options->ksMin = getNum(Options::DEFAULT_KS_MIN, "Keyspace - Minimum Length");
    if (options->ksMin < Options::KS_MIN || options->ksMin > Options::KS_MAX){
        throw "Invalid keyspace - minimum length must be between " + to_string(Options::KS_MIN) + " and " + to_string(Options::KS_MAX) + ".";
    }
    options->ksMax = getNum(Options::DEFAULT_KS_MAX, "Keyspace - Maximum Length");
    if (options->ksMax < options->ksMin || options->ksMax > Options::KS_MAX){
        throw "Invalid keyspace - maximum length must be between " + to_string(options->ksMin) + " and " + to_string(Options::KS_MAX) + ".";
    }
    options->ksUseLower = getBool(Options::DEFAULT_KS_USE_LOWER, "Keyspace - Lowercase");
    options->ksUseUpper = getBool(Options::DEFAULT_KS_USE_UPPER, "Keyspace - Uppercase");
    options->ksUseNum = getBool(Options::DEFAULT_KS_USE_NUM, "Keyspace - Numbers");
    if (options->ksUseNum){
        options->optMaxCombinedNums = getNum(Options::DEFAULT_OPT_MAX_COMBINED_NUMS, "Option - Maximum Combined Numbers (Example (4) = 'password12' = 'password1234')");
    }
    options->optUseLeet = getBool(Options::DEFAULT_OPT_LEET, "Option - Leet Substitution (Example 'e' > '3')");
    options->optUsePrepend = getBool(Options::DEFAULT_OPT_USE_PREPEND, "Option - Prepend sequences (Example 'password' = '123password')");
    if (options->optUsePrepend){
        options->optPrepend = getString(Options::DEFAULT_OPT_PREPEND, "Option - Prepend Sequences (Comma Separated)");
    }
    options->optUseAppend = getBool(Options::DEFAULT_OPT_USE_APPEND, "Option - Append sequences (Example 'password' = 'password123')");
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