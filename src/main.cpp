#include <iostream>
#include <conio.h>

#include "leet.h"
#include "options.h"
#include "filebuffer.h"
#include "generator.h"

using namespace std;

const string DEFAULT_OUTPUT = "./output.txt";
const string DEFAULT_LEET_CONFIG = "./leet.config";

enum MODE {
    MODE_HELP,
    MODE_RUN,
    MODE_CONFIG
};
enum ARGUMENT_INDEX {
    ARGUMENT_INDEX_HELP1 = 0,
    ARGUMENT_INDEX_HELP2 = 1,
    ARGUMENT_INDEX_HELP3 = 2,
    ARGUMENT_INDEX_HELP4 = 3,
    ARGUMENT_INDEX_OUTPUT = 4,
    ARGUMENT_INDEX_GENERATE_LEET = 5,
    ARGUMENT_INDEX_USE_LEET = 6,
    ARGUMENT_INDEX_THREADS = 7
};
const char* ARGUMENT_FLAGS[] = {
    "?",
    "-?",
    "help",
    "-help",
    "-o",
    "-gl",
    "-l",
    "-t"
};

struct arguments {
    MODE mode = MODE_RUN;
    string outputFile = DEFAULT_OUTPUT;
    string leetConfig = DEFAULT_LEET_CONFIG;
    int threads = Generator::DEFAULT_NUM_THREADS;
};

shared_ptr<Options> getOptions();
shared_ptr<Options> getOptions(const string leetConfigData);
arguments getArguments(int argc, char **argv);
bool run(const arguments args);
bool generateLeet(const arguments args);
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
    cout << endl;
    cout << "PassSniper v1.0.0 - AJ Savino" << endl;
    cout << "Generating targeted wordlists for pentesting" << endl;
    cout << endl;
    cout << "CLI Usage:" << endl;
    cout << "  passsniper <output>" << endl;
    cout << "    -?  Show commands" << endl;

    arguments args = getArguments(argc, argv);

    int exitCode = 0;
    string errorMsg;
    try {
        switch (args.mode){
            default:
            case MODE_HELP:
                cout << "  passsniper [Options]" << endl;
                cout << "    -o  <output>: File to output wordlist" << endl;
                cout << "    -l  <leetConfig>: File containing leet config" << endl;
                cout << "    -gl <leetConfig>: Generate leet config file" << endl;
                cout << "    -t  <numThreads>: Number of threads to use. Pass 0 to disable threading" << endl;
                break;
            case MODE_RUN:
                args.outputFile = getString(args.outputFile, "Output File");
                exitCode = run(args);
                break;
            case MODE_CONFIG: 
                args.leetConfig = getString(args.leetConfig, "Leet File");
                exitCode = generateLeet(args);
                break;
        }
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
        if (!exitCode){
            exitCode = 1;
        }
    }
    
    cout << endl;
    system("PAUSE");
    return exitCode;
}

arguments getArguments(int argc, char **argv){
    arguments args;
    if (argc >= 2){
        bool foundFlag = false;
        for (int i = 0; i < argc; i++){
            string arg = argv[i];
            if (arg == ARGUMENT_FLAGS[ARGUMENT_INDEX_HELP1] || 
                arg == ARGUMENT_FLAGS[ARGUMENT_INDEX_HELP2] ||
                arg == ARGUMENT_FLAGS[ARGUMENT_INDEX_HELP3] ||
                arg == ARGUMENT_FLAGS[ARGUMENT_INDEX_HELP4]){
                foundFlag = true;
                args.mode = MODE_HELP;
                break;
            }
            if (arg == ARGUMENT_FLAGS[ARGUMENT_INDEX_GENERATE_LEET]){
                foundFlag = true;
                args.mode = MODE_CONFIG;
                if (i + 1 < argc){
                    args.leetConfig = argv[i + 1];
                }
                break;
            }
            if (arg == ARGUMENT_FLAGS[ARGUMENT_INDEX_USE_LEET] && i + 1 < argc){
                foundFlag = true;
                args.leetConfig = argv[i + 1];
            }
            if (arg == ARGUMENT_FLAGS[ARGUMENT_INDEX_OUTPUT] && i + 1 < argc){
                foundFlag = true;
                args.outputFile = argv[i + 1];
            }
            if (arg == ARGUMENT_FLAGS[ARGUMENT_INDEX_THREADS] && i + 1 < argc){
                foundFlag = true;
                args.threads = stoi(argv[i + 1]);
            }
        }
        if (!foundFlag && argc == 2){
            args.outputFile = argv[1];
        }
    }
    return args;
}

bool run(const arguments args){
    //Warning
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

    //Test file access
    shared_ptr<FileBuffer> fb (new FileBuffer(args.outputFile));
    if (!fb->test()){
        throw "Could not open output file.";
    }

    string leetConfigData = "";
    ifstream ifs;
    ifs.exceptions(ios::failbit | ios::badbit);
    try {
        ifs.open(args.leetConfig, ios::in); //Open file for input
        if (ifs && ifs.is_open()){
            string line;
            while (getline(ifs, line)){
                leetConfigData += line + '\n';
            }
            ifs.close();
            cout << endl << "Loaded leet config." << endl;
        }
    } catch (...){}

    //Get user options
    shared_ptr<Options> options = getOptions(leetConfigData);
    cout << endl << options << endl;

    //Generate
    unique_ptr<Generator> gen (new Generator(options, fb));
    gen->generate(args.threads);

    return 0;
}

bool generateLeet(const arguments args){
    ofstream ofs;
    ofs.exceptions(ios::failbit | ios::badbit);
    ofs.open(args.leetConfig, ios::out | ios::trunc); //Open file for output and append to eof
    if (ofs && ofs.is_open()){
        ofs << Leet::CONFIG;
        ofs.close();
    } else {
        throw "There was an error opening the output file.";
        return 1;
    }
    return 0;
}

shared_ptr<Options> getOptions(){
    return getOptions("");
}
shared_ptr<Options> getOptions(const string leetConfigData){
    shared_ptr<Options> options (new Options(Leet::parse(leetConfigData)));
    
    //Target
    bool isDemo;
    bool isOrganization;
    bool isIndividual;
    do {
        string target;
        cout << endl;
        cout << "Select wordlist generation target:" << endl;
        cout << "  [0] Demo - Organization" << endl;
        cout << "  [1] Organization" << endl;
        cout << "  [2] Invididual" << endl;
        cout << ":";
        cin.clear();
        cin.sync();
        getline(cin, target);

        isDemo = target == "0";
        isOrganization = target == "1";
        isIndividual = target == "2";
        if (isDemo || isOrganization || isIndividual){
            break;
        } else {
            cout << endl << "Please type '0', '1' or '2' and press [ENTER]" << endl;
        }
    } while (true);
    if (isDemo){
        options->demoOrganization();
        return options;
    }
    options->dataIsOrganization = isOrganization;
    options->dataIsIndividual = isIndividual;

    cout << endl << "LESS is MORE! Filling in everything will generate MILLIONS of combinations" << endl;

    //Names
    cout << endl;
    cout << "Names (Optional)(Comma Separated)" << endl;
    if (options->dataIsOrganization){
        cout << "      (Company,Street,Other)" << endl;
    }
    if (options->dataIsIndividual){
        cout << "      (Pets,Children,Spouse,Other)" << endl;
    }
    options->dataNames = getString();

    //Keywords
    cout << endl;
    cout << "Keywords (Optional)(Comma Separated)" << endl;
    if (options->dataIsOrganization){
        cout << "         (Mascot,Products,Other)" << endl;
    }
    if (options->dataIsIndividual){
        cout << "         (Sports,Teams,Animals,Interests,Other)" << endl;
    }
    options->dataKeywords = getString();

    //Dates
    cout << endl;
    cout << "Significant Dates (Optional)(Comma Separated)(mm-dd-yyyy)" << endl;
    if (options->dataIsOrganization){
        cout << "                  (Founded,Other)" << endl;
    }
    if (options->dataIsIndividual){
        cout << "                  (Anniversary,Birthdays,Graduation,Other)" << endl;
    }
    options->dataDates = getString();

    //Numbers
    cout << endl;
    cout << "Significant Numbers (Optional)(Comma Separated)(555-0157)" << endl;
    if (options->dataIsOrganization){
        cout << "                    (Street,Phone,ID,Other)" << endl;
    }
    if (options->dataIsIndividual){
        cout << "                    (Favorite,Street,Phone,Other)" << endl;
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