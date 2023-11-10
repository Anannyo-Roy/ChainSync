#ifndef AddNode_h
#define AddNode_h

#include<iostream>
#include"MySQLDatabase.h"
#include<vector>
using namespace std;
using namespace sql;

class AddNode {
private:
	MySQLDatabase db;
	vector<string> intermerdiates;
public:
	AddNode();
	void getInter();
	string vectorToString(const vector<int>& numbers);
	vector<int> stringToVector(const string& input);
	string currid();
	void FarmAdd();
	void farmname();
	void farmsize();
	void storagename();
	void storagecapacity();
	void storagemaxcapacity();
	void StorageAdd();


};


#endif