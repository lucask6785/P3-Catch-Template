#include "CampusCompass.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <queue>
#include <unordered_set>
#include <algorithm>

using namespace std;

CampusCompass::CampusCompass() {
    // initialize your object
}

bool CampusCompass::ParseCSV(const string &edges_filepath, const string &classes_filepath) {
    // return boolean based on whether parsing was successful or not
    ifstream file(edges_filepath);
    if (!file.is_open()) {
        // Handle error: file could not be opened
        return false;
    }
    string line;
    while (getline(file, line)) {
        vector<string> parsed;
        string word;
        stringstream line_stream;
        while  (getline(line_stream, word, ',')) {
            parsed.push_back(word);
        }
        if (this->locations_graph.count(stoi(parsed[0]))) {
            this->locations_graph[stoi(parsed[0])].push_back({stoi(parsed[1]), stoi(parsed[4]), 1});
        } else {
            this->locations_graph[stoi(parsed[0])] = {};
            this->locations_graph[stoi(parsed[0])].push_back({stoi(parsed[1]), stoi(parsed[4]), 1});
        }
        if (this->locations_graph.count(stoi(parsed[1]))) {
            this->locations_graph[stoi(parsed[1])].push_back({stoi(parsed[0]), stoi(parsed[4]), 1});
        } else {
            this->locations_graph[stoi(parsed[1])] = {};
            this->locations_graph[stoi(parsed[1])].push_back({stoi(parsed[0]), stoi(parsed[4]), 1});
        }
    }
    file.close();

    ifstream file2(classes_filepath);
    if (!file2.is_open()) {
        // Handle error: file could not be opened
        return false;
    }
    while (getline(file2, line)) {
        vector<string> parsed;
        string word;
        stringstream line_stream;
        while (getline(line_stream, word, ',')) {
            parsed.push_back(word);
        }
        this->class_map[parsed[0]] = stoi(parsed[1]);
    }
    file2.close();

    return true;
}

bool CampusCompass::ParseCommand(const string &command) {
    // do whatever regex you need to parse validity
    // hint: return a boolean for validation when testing. For example:
    bool is_valid = true; // replace with your actual validity checking

    return is_valid;
}

bool CampusCompass::InsertStudent(string name, string id, string residence, int n, vector<string>& codes) {
    if (this->students_map.count(id)) {
        return false;
    }
    this->students_map[id] = {name, residence};
    for (int i = 0; i < n; i++) {
        this->students_map[id].push_back(codes[i]);
    }
    return true;
}

bool CampusCompass::RemoveStudent(string id) {
    if (!this->students_map.count(id)) {
        return false;
    }
    this->students_map.erase(id);
    return true;
}

bool CampusCompass::DropClass(string id, string code) {
    if (!this->students_map.count(id)) {
        return false;
    }
    for (int i = 2; i < this->students_map[id].size() - 2; i++) {
        if (this->students_map[id][i] == code) {
            this->students_map[id].erase(this->students_map[id].begin() + i);
            if (this->students_map[id].size() == 2) {
                this->students_map.erase(id);
            }
            return true;
        }
    }
    return false;
}

bool CampusCompass::ReplaceClass(string id, string code1, string code2) {
    if (!this->students_map.count(id)) {
        return false;
    }
    for (int i = 2; i < this->students_map[id].size() - 2; i++) {
        if (this->students_map[id][i] == code2) {
            return false;
        }
    }
    for (int i = 2; i < this->students_map[id].size() - 2; i++) {
        if (this->students_map[id][i] == code1) {
            this->students_map[id].erase(this->students_map[id].begin() + i);
            this->students_map[id].push_back(code2);
            return true;
        }
    }
    return false;
}

int CampusCompass::RemoveClass(string code) {
    int dropped = 0;
    for (auto& pair : this->students_map) {
        for (int i = 2; i < pair.second.size() - 2; i++) {
            if (pair.second[i] == code) {
                pair.second.erase(pair.second.begin() + i);
                dropped++;
                if (pair.second.size() == 2) {
                    this->students_map.erase(pair.first);
                }
            }
        }
    }
    return dropped;
}

bool CampusCompass::ToggleEdgesClosure(int n, vector<pair<int, int>>& edges) {
    for (int i = 0; i < n; i++) {
        pair<int, int> edge = edges[i];
        for (int i = 0; i < this->locations_graph[edge.first].size(); i++) {
            if (this->locations_graph[edge.first][i][0] == edge.second) {
                if (this->locations_graph[edge.first][i][2] == 1) {
                    this->locations_graph[edge.first][i][2] = 0;
                } else {
                    this->locations_graph[edge.first][i][2] = 1;
                }
            }
        }
        for (int i = 0; i < this->locations_graph[edge.second].size(); i++) {
            if (this->locations_graph[edge.second][i][0] == edge.first) {
                if (this->locations_graph[edge.second][i][2] == 1) {
                    this->locations_graph[edge.second][i][2] = 0;
                } else {
                    this->locations_graph[edge.second][i][2] = 1;
                }
            }
        }
    }
    return true;
}

string CampusCompass::CheckEdgeStatus(int location1, int location2) {
    for (int i = 0; i < this->locations_graph[location1].size(); i++) {
        if (this->locations_graph[location1][i][0] == location2) {
            if (this->locations_graph[location1][i][2] == 1) {
                return "open";
            } else {
                return "closed";
            }
        }
    }
    return "DNE";
}

bool CampusCompass::IsConnected(int location1, int location2) {
    unordered_set<int> visited;
    queue<int> q;

    visited.insert(location1);
    q.push(location1);

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        if (current == location2) {
            return true;
        }

        for (int i = 0; i < this->locations_graph[current].size(); i++) {
            if (!visited.count(this->locations_graph[current][i][0])) {
                q.push(this->locations_graph[current][i][0]);
                visited.insert(this->locations_graph[current][i][0]);
            }
        }
    }
    return false;
}

struct CompareDist {
    bool operator()(pair<int, int>& a, pair<int, int>& b) {
        return a.first > b.first;
    }
};

void CampusCompass::PrintShortestEdges(string id) {
    unordered_map<int, int> dist;
    unordered_set<int> visited;
    int residence = stoi(this->students_map[id][1]);
    dist[residence] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, CompareDist> pq;
    pq.push({0, residence});

    while (!pq.empty()) {
        pair<int, int> current = pq.top();
        pq.pop();
        int d = current.first;
        int u = current.second;
        visited.insert(u);

        if (!dist.count(u)) {
            dist[u] = d;
        } else {
            if (dist[u] < d) {
                continue;
            }
        }

        for (int i = 0; i < this->locations_graph[u].size(); i++) {
            if (!visited.count(this->locations_graph[u][i][0]) && this->locations_graph[u][i][2] == 1) {
                int v = this->locations_graph[u][i][0];
                int weight = this->locations_graph[u][i][1];

                if (!dist.count(v)) {
                    dist[v] = dist[u] + weight;
                } else {
                    if (dist[u] + weight < dist[v]) {
                        dist[v] = dist[u] + weight;
                    }
                }

                pq.push({dist[v], v});
            }
        }
    }

    vector<string> s;
    for (int i = 2; i < this->students_map[id].size() - 2; i++) {
        s.push_back(this->students_map[id][i]);
    }
    sort(s.begin(), s.end());

    cout << "Name: " << this->students_map[id][0] << endl;
    for (int i = 0; i < s.size(); i++) {
        if (!dist.count(this->class_map[s[i]])) {
            dist[this->class_map[s[i]]] = -1;
        }
        cout << s[i] << " | Total Time: " << dist[this->class_map[s[i]]] << endl;
    }
}

