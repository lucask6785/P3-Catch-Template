#pragma once
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class CampusCompass {
private:
    unordered_map<int, vector<vector<int>>> locations_graph; // {Location ID : {{Location ID, Time, Open}}}
    unordered_map<string, vector<string>> students_map; // Map of classes each student is in {name, residence}
    unordered_map<string, int> class_map; // Map class code to location ID
    
public:
    // Think about what helper functions you will need in the algorithm
    CampusCompass(); // constructor
    bool ParseCSV(const string &edges_filepath, const string &classes_filepath);
    bool ParseCommand(const string &command);
    bool InsertStudent(string name, string id, string residence, int n, vector<string>& codes);
    bool RemoveStudent(string id);
    bool DropClass(string id, string code);
    bool ReplaceClass(string id, string code1, string code2);
    int RemoveClass(string code);
    bool ToggleEdgesClosure(int n, vector<pair<int, int>>& edges);
    string CheckEdgeStatus(int location1, int location2);
    bool IsConnected(int location1, int location2);
    void PrintShortestEdges(string id);
    void PrintStudentZone(string id);
};
