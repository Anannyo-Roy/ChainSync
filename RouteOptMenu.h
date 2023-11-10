#ifndef RouteOptMenu_h
#define RouteOptMenu_h
#include<vector>
#include"MySQLDatabase.h"


class RouteOptMenu {
private:
	MySQLDatabase db;
	int amount;
	vector<string> storages;
public:

	RouteOptMenu();
	string currid();
	void menu();

};

#endif