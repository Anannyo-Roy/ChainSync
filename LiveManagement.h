#pragma once
#ifndef livemanagement_h
#define livemanagement_h

#include <iostream>
#include <string>
#include <eigen/dense>
using namespace std;

class LiveManagement {
public:
	//livemanagement();
	LiveManagement(string managerid = "", int storageMaxCapacity = 0, int storageCapacity = 0);


	string getmanagerid();
	void setmanagerid(string managerid);
	int getStorageMaxCapacity();
	void setStorageMaxCapacity(int storageMaxCapacity);
	int getStorageCapacity();
	void setStorageCapacity(int storageCapacity);

	// methods
	void LMMenu(string managerid, int storagemaxcapacity, int storagecapacity);

private:
	string managerid;
	int storageMaxCapacity;
	int storageCapacity;
};

#endif