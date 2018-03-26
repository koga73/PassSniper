#include <iostream>
#include <conio.h>
#include <string>
#include <sstream>

using namespace std;

void getOptions();
int getNum(int defaultNum, string message);

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
    
    getOptions();

    system("PAUSE");
}

void getOptions(){
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

    //Names
    string names;
    cout << endl;
    cout << "Names (Optional)(Comma Separated)" << endl;
    if (isIndividual){
        cout << "      (Pets,Children,Spouse,Other)" << endl;
    }
    if (isOrganization){
        cout << "      (Company,Street,Other)" << endl;
    }
    cout << ":";
    cin.clear();
    cin.sync();
    getline(cin, names);

    //Keywords
    string keywords;
    cout << endl;
    cout << "Keywords (Optional)(Comma Separated)" << endl;
    if (isIndividual){
        cout << "         (Sports,Teams,Animals,Interests,Other)" << endl;
    }
    if (isOrganization){
        cout << "         (Mascot,Products,Other)" << endl;
    }
    cout << ":";
    cin.clear();
    cin.sync();
    getline(cin, keywords);

    //Dates
    string dates;
    cout << endl;
    cout << "Significant Dates (Optional)(Comma Separated)(mm-dd-yyyy)" << endl;
    if (isIndividual){
        cout << "                  (Anniversary,Birthdays,Other)" << endl;
    }
    if (isOrganization){
        cout << "                  (Founded,Other)" << endl;
    }
    cout << ":";
    cin.clear();
    cin.sync();
    getline(cin, dates);

    //Numbers
    string numbers;
    cout << endl;
    cout << "Significant Numbers (Optional)(Comma Separated)(555-5555)" << endl;
    if (isIndividual){
        cout << "                    (Favorite,Street,Phone,Other)" << endl;
    }
    if (isOrganization){
        cout << "                    (Street,Phone,ID,Other)" << endl;
    }
    cout << ":";
    cin.clear();
    cin.sync();
    getline(cin, numbers);

    //Keyspace min
    int min = getNum(7, "Keyspace - Minimum Length (Required)");

    //Keyspace max
    int max = getNum(15, "Keyspace - Maximum Length (Required)");

    cout << min << " " << max << endl;
}

int getNum(int defaultNum, string message){
    int num = defaultNum;
    do {
        string input;
        cout << endl << message << endl;
        cout << ": (" << num << ") ";
        cin.clear();
        cin.sync();
        getline(cin, input);
        if (!input.length()){ //Default
            break;
        }
        try {
            num = stoi(input);
            break;
        } catch (exception ex){
            cout << endl << "Please type a valid number" << endl;
        }
    } while (true);
    return num;
}