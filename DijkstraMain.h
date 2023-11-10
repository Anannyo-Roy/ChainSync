#ifndef DijkstraMain_H
#define DijkstraMain_H

#include <iostream>
#include <climits>
#include <vector>
#include <map>
#include<sstream>
#include"MySQLDatabase.h"



using namespace std;
using namespace sql;


class RouteOpt {
private:
    string source, target;
    int sourceind, targetind;
    vector<vector<int>> graph;
    int graphsize;
    vector<string> vertexnames;
    MySQLDatabase db;

public:
    RouteOpt(string sc, string tg);
    vector<int> stringToVector(const string& input);
    string vectorToString(const vector<int>& numbers);
    void getGraph();
    void getVertexNames();
    void getSourceAndTargetIndex();
    int getVertexIndex(string a);
    int miniDist(vector<int>& distance, vector<bool>& Tset);
    void printPath(vector<int>& parent, int j, vector<string>vertexNames);
    bool DijkstraAlgo(int flag = 0, string t = "");


};


#endif