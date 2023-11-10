#include <iostream>
#include <climits>
#include <vector>
#include <map>
#include<sstream>
#include"MySQLDatabase.h"
#include"DijkstraMain.h"

using namespace std;
using namespace sql;

//MySQLDatabase db;

    RouteOpt::RouteOpt(string sc, string tg) {
        this->source = sc;
        this->target = tg;
        db = MySQLDatabase("tcp://localhost:3306", "root", "1234", "projtest");
    }

    //WILL BE USED FOR INPUT OF FARM AND STORAGE LATER ON
    vector<int> RouteOpt::stringToVector(const string& input) {
        vector<int> result;
        istringstream iss(input);
        string token;

        while (getline(iss, token, ',')) {
            int number;
            istringstream(token) >> number; // Convert the token to an integer
            result.push_back(number);
        }

        return result;
    }

    //WILL BE ALSO USED FOR ADDING LATER ON
    string RouteOpt::vectorToString(const vector<int>& numbers) {
        string result;
        for (size_t i = 0; i < numbers.size(); i++) {
            result += to_string(numbers[i]);
            if (i < numbers.size() - 1) {
                result += ",";
            }
        }
        return result;
    }

    //Constructing the graph
    void RouteOpt::getGraph() {
        //MySQLDatabase db("tcp://localhost:3306", "root", "1234", "projtest");
        if (db.Connect()) {
            try {
                
                string FarmerID;
                string q;
                int dimen;

                //Getting the maximum dimension ie. count of locations + location of farm (1)
                q = "Select count(*) from Locations";
                ResultSet* rs3 = db.SelectData(q);
                if (rs3) {
                    while (rs3->next()) {
                        dimen = rs3->getInt(1);
                    }
                }
                dimen++; // adding the location of farm

                //Getting Farmer id
                q = "Select id from CurrentUser";
                ResultSet* rs1 = db.SelectData(q);
                if (rs1) {
                    while (rs1->next()) {
                        FarmerID = rs1->getString("id");
                    }
                }

                //FOR FARMER TABLE
                q = "Select farmloc from FarmerTable where farmerid='" + FarmerID + "'";
                ResultSet* rs2 = db.SelectData(q);
                if (rs2) {
                    while (rs2->next()) {
                        string t = rs2->getString("farmloc");
                        vector<int> temp = stringToVector(t);
                        //Matching Dimensions
                        int tempsize = temp.size();
                        while (tempsize < dimen) {
                            temp.push_back(0);
                            tempsize += 1;
                        }
                        //adding the farm location to the vector first
                        this->graph.push_back(temp);
                    }
                }
                
                //FOR LOCAITONS TABLE
                q = "Select loc from locations;";
                ResultSet* rs = db.SelectData(q);

                if (rs) {
                    while (rs->next()) {
                        string t = rs->getString("loc");
                        //Matching dimensions
                        vector<int> temp = stringToVector(t);
                        int tempsize = temp.size();

                        while (tempsize < dimen) {
                            temp.push_back(0);
                            tempsize += 1;
                        }
                        //Pushing to main graph vector after matching the bits 
                        this->graph.push_back(temp);
                    }
                }
                //test
                //for (vector<int> i : graph) {
                //    for (int x : i) {
                //        cout << x <<" ";
                //    }
                //    cout << endl;
                //}

            }
            catch (sql::SQLException e) {
                cerr << "Sql error : " << e.what() << endl;
            }
        }

    }

    void RouteOpt::getVertexNames() {
        //MySQLDatabase db("tcp://localhost:3306", "root", "1234", "projtest");
        string q;
        string FarmerID;
        

        //Getting Farmer id
        q = "Select id from CurrentUser";
        ResultSet* rs = db.SelectData(q);
        if (rs) {
            while (rs->next()) {
                FarmerID = rs->getString("id");
            }
        }
        
        //Getting farm name
        q = "Select farmname from FarmerTable where farmerid='" + FarmerID + "'";
        ResultSet* rs1 = db.SelectData(q);
        if (rs1) {
            while (rs1->next()) {
                this->vertexnames.push_back(rs1->getString("farmname"));
            }
        }

        //Getting all the other location names
        q = "Select name from locations";
        ResultSet* rs2 = db.SelectData(q);
        if (rs2) {
            while (rs2->next()) {
                this->vertexnames.push_back(rs2->getString("name"));
            }
        }

        //test
      /*  for (int i = 0; i < vertexnames.size(); i++) {
            for (int x : graph[i]) {
                cout << x << " ";
            }
            cout << vertexnames[i] << endl;
        }*/

    }

    void RouteOpt::getSourceAndTargetIndex() {
        for (int i = 0; i < vertexnames.size(); i++) {
            if (vertexnames[i] == source) {
                this->sourceind = i;
            }
            if (vertexnames[i] == target) {
                this->targetind = i;
            }
        }   
    }

    int RouteOpt::getVertexIndex(string a) {

        //test
        //for (string i : vertexnames) {
        //    cout << i << endl;
        //}
        for (int i = 0; i < vertexnames.size(); i++) {
            if (vertexnames[i] == a) {
                //cout << vertexnames[i] << " and its index " << i;
                return i;
            }
        }
    }


    int RouteOpt::miniDist(vector<int>& distance, vector<bool>& Tset) {
        int minimum = INT_MAX, ind;
        for (int k = 0; k < distance.size(); k++) {
            if (!Tset[k] && distance[k] <= minimum) {
                minimum = distance[k];
                ind = k;
            }
        }
        return ind;
    }

    void RouteOpt::printPath(vector<int>& parent, int j, vector<string>vertexNames) {
        if (parent[j] == -1)
            return;

        printPath(parent, parent[j], vertexNames);


        cout << "\t\t   | " << endl;
        cout << "\t\t   |  " <<graph[parent[j]][j]<<"km" << endl;
        cout << "\t\t   V " << endl;
        cout << "\t\t " << vertexnames[j] << endl;

    }

    bool RouteOpt::DijkstraAlgo(int flag, string t ) {  //flag 1 then shortest
        if (graph.empty()) {
            getGraph();
        }
        if (vertexnames.empty()) {
            getVertexNames();
        }
        getSourceAndTargetIndex();
        
        int V = graph.size();
        vector<int> distance(V, INT_MAX);
        vector<bool> Tset(V, false);
        vector<int> parent(V, -1);

        distance[sourceind] = 0;

        for (int k = 0; k < V; k++) {
            int m = miniDist(distance, Tset);
            Tset[m] = true;

            for (int k = 0; k < V; k++) {
                if (!Tset[k] && graph[m][k] && distance[m] != INT_MAX && distance[m] + graph[m][k] < distance[k]) {
                    distance[k] = distance[m] + graph[m][k];
                    parent[k] = m;
                }
            }
        }

        if (flag == 0) {
            cout << "ROUTE : [Farm: " + source + "] --> [Storage: " + target + "]" << endl;
            cout << endl;
            cout << "The most Optimized and Shortest route is as following : " << endl;
            cout << endl;

            cout <<"\t\t" << vertexnames[sourceind] << endl;
            printPath(parent, targetind, vertexnames);
            cout << endl<<endl;

            cout << "Total Distance from [Farm: " + source + "] --> [Storage: " + target + "] is " << distance[targetind] << "KM" << endl;

            return true;
        }
        else {

            //chcking values of distance
            /*for (int i = 0; i < distance.size(); i++) {
                cout << distance[i] << vertexnames[i] << endl;
            }cout << endl;*/
            
            int mindist = INT_MAX;
            int ind;
            for (int i = 1; i < vertexnames.size(); i++) {
                if (distance[i] < mindist) {
                    mindist = distance[i];
                    ind = i;
                }
            }

            if (getVertexIndex(t) == ind) {
                return true;
            }
            else {
                return false;
            }
        }

    }
