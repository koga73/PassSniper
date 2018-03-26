#include <iostream>
#include <string>
#include <sstream>

using namespace std;

void getOptions();

int main(int argc, char **argv){
    getOptions();

    system("PAUSE");
}

void getOptions(){
    cout << "    ____                  _____       _                " << endl;
    cout << "   / __ \____ ___________/ ___/____  (_)___  ___  _____" << endl;
    cout << "  / /_/ / __ `/ ___/ ___/\__ \/ __ \/ / __ \/ _ \/ ___/" << endl;
    cout << " / ____/ /_/ (__  |__  )___/ / / / / / /_/ /  __/ /    " << endl;
    cout << "/_/    \__,_/____/____//____/_/ /_/_/ .___/\___/_/     " << endl;
    cout << "                                   /_/                 " << endl;
    cout << "v1.0.0" << endl;
    cout << "By: AJ Savino" << endl;
    cout << endl;
    cout << "Generating better quality wordlists for pentesting" << endl;
    cout << endl;

    bool isIndividual;
    bool isOrganization;
    do {
        cout << endl;
        cout << "Select wordlist generation target:" << endl;
        cout << "  (1) Invididual" << endl;
        cout << "  (2) Organization" << endl;
        cout << ":";

        char target;
        cin >> target;
        isIndividual = target == '1';
        isOrganization = target == '2';
    } while (!isIndividual && !isOrganization);

    string names;
    cout << endl;
    cout << "Names (Optional)(Comma Separated)" << endl;
    if (isIndividual){
        cout << "      (Pets,Children,Spouse)" << endl;
    }
    if (isOrganization){
        cout << "      (Company,Street)" << endl;
    }
    cout << ":";
    cin.ignore();
    getline(cin, names);

    string keywords;
    cout << endl;
    cout << "Keywords (Optional)(Comma Separated)" << endl;
    if (isIndividual){
        cout << "         (Sports,Teams,Animals,Interests)" << endl;
    }
    if (isOrganization){
        cout << "         (Mascot,Products)" << endl;
    }
    cout << ":";
    cin.ignore();
    getline(cin, keywords);

    string dates;
    cout << endl;
    cout << "Significant Dates (Optional)(Comma Separated)(m/d/yyyy)" << endl;
    if (isIndividual){
        cout << "                  (Anniversary,Birthdays)" << endl;
    }
    if (isOrganization){
        cout << "                  (Founded)" << endl;
    }
    cout << ":";
    cin.ignore();
    getline(cin, dates);

    string numbers;
    cout << endl;
    cout << "Significant Numbers (Optional)(Comma Separated)(555-5555)" << endl;
    if (isIndividual){
        cout << "                    (Favorite,Street,Phone)" << endl;
    }
    if (isOrganization){
        cout << "                    (Street,Phone,ID)" << endl;
    }
    cout << ":";
    cin.ignore();
    getline(cin, numbers);
}