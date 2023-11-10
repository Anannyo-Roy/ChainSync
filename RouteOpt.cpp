#include<iostream>
#include"MySQLDatabase.h"
#include"DijkstraMain.h"
#include"RouteOptMenu.h"

using namespace std;
using namespace sql;


	RouteOptMenu::RouteOptMenu() {
		db = MySQLDatabase("tcp://localhost:3306", "root", "1234", "projtest");

		if (db.Connect()) {
			try {
				//getting all the storages
				string q;
				q = "select storagename from managertable";
				ResultSet* rs = db.SelectData(q);
				if (rs) {
					while (rs->next()) {
						storages.push_back(rs->getString("storagename"));
					}
				}

			}
			catch (sql::SQLException e) {
				cerr << "SQL error : " << e.what() << endl;
			}
		}
	}

	string RouteOptMenu::currid() {
		if (db.Connect()) {
			try {
				string q;
				string id;
				q = "select id from currentuser";
				ResultSet* rs = db.SelectData(q);
				if (rs) {
					while (rs->next()) {
						id = rs->getString("id");
					}
				}
				return id;
			}
			catch (sql::SQLException e) {
				cerr << "SQL error : " << e.what() << endl;
			}
		}
	}

	void RouteOptMenu::menu() {
		if (db.Connect()) {
			string q;
			string id = currid();
			try {
				string source;
				q = "select farmname from farmertable where farmerid='" + id + "'";
				ResultSet* rs = db.SelectData(q);
				if (rs) {
					while (rs->next()) {
						source = rs->getString("farmname");
					}
				}
				int choice;
				
				cout << "ROUTE OPTIMIZATION" << endl<<endl;
				int count = 0;
				cout << "Select the desired storage :" << endl;
				for (string i : storages) {
					cout << ++count << ". " << i << "\t";
					RouteOpt obj1(source,i);
					if (obj1.DijkstraAlgo(1, i)) {
						cout << "(Recommended as closest)" << endl;
					}
				}
				cout<<endl;
				cout << "(Any other option to go back)";
				cout << endl<<endl;
				cout << "Storage chosen (only option no.): ";
				cin >> choice;
				cout << endl << endl;

				if (choice > 0 && choice <= count) {
					
					string target =storages[count-1];
					RouteOpt obj(source,target);
					obj.DijkstraAlgo();


				}
				else {
					cout << "BACK.";
				}

			}
			catch (sql::SQLException e) {
				cerr << "SQL error : " << e.what() << endl;
			}
		}
	}