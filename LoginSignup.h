#ifndef LoginSignup_h
#define LoginSignup_h

#include<iostream>
#include"MySQLDatabase.h"
using namespace std;
using namespace sql;

class LoginSignup {
private:
	MySQLDatabase db;
public:
	LoginSignup();
	void OccupationChoice();
	int LoginRegisterChoice();
	string occupat();
	string currid();
	bool login();
	bool isStrongPassword(string password);
	bool registering();
	bool registerfarmer();
	bool registermanager();

};


#endif