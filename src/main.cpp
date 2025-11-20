#include "CampusCompass.h"
#include <iostream>

using namespace std;

int main() {
    // initialize your main project object
    CampusCompass compass;

    // ingest CSV data
    compass.ParseCSV("../data/edges.csv", "../data/classes.csv");

    /*
    // the below is example code for parsing commandline input
    int no_of_lines;
    string command;
    cin >> no_of_lines;
    cin.ignore(); // ignore newline that first cin left over
    for (int i = 0; i < no_of_lines; i++) {
        getline(cin, command);

        // parse your commands however you see fit
        compass.ParseCommand(command);
    }
    */

    vector<string> s = {"COP3530", "MAC2311"};
    cout << compass.InsertStudent("Brandon", "45679999", "20", 2, s) << endl;
    s = {"COP3530", "CDA3101", "MAC2311"};
    cout << compass.InsertStudent("Brian", "35459999", "21", 3, s) << endl;
    s = {"CDA3101", "MAC2311", "EEL3701"};
    cout << compass.InsertStudent("Briana", "87879999", "22", 3, s) << endl;
    cout << compass.RemoveClass("COP3530") << endl;
    cout << compass.RemoveStudent("87879999") << endl;
    cout << compass.RemoveClass("CDA3101") << endl;
    compass.PrintShortestEdges("35459999");
}
