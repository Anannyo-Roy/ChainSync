#include<iostream>
#include"LoginSignup.h"
#include"AddNode.h"
#include"DijkstraMain.h"
#include"RouteOptMenu.h"
using namespace std;

int main() {
	cout << "\t\tCHAIN SYNC\t\t" << endl;
	cout << "-----------------------------------------------\n";
	while (true) {
		LoginSignup obj;
		
		obj.OccupationChoice();
		cout << endl;
		int a = obj.LoginRegisterChoice();
		cout << endl;
		if (a == 1) {
			if (obj.login()) {
				while (true) {
					string id = obj.occupat();
					if (id == "farmer") {
						while(true){
							int choice;
							cout << endl;
							cout << "\t\tWELCOME FARMER" << endl;
							cout << "1. Add Farm" << endl;
							cout << "2. Personal Details" << endl;
							cout << "3. Route Optimization" << endl;
							cout << "4. Prediction Model" << endl;
							cout << "5. Exit" << endl;
							cout << endl;
							cout << "Enter desired choice: ";
							cin >> choice;

							if (choice == 1) {
								AddNode obj1;
									cout << "\t\tADDING / UPDATING FARM" << endl << endl;
									obj1.FarmAdd();
									cout << endl;
							}
							else if (choice == 2) {
								//personal details and update
							}
							else if (choice == 3) {
								RouteOptMenu obj2;
								obj2.menu();
								cout << endl;
							}
							else if (choice == 4) {
								//prediction model
							}
							else if (choice == 5) {
								break;
							}
						}
						break;
					}
					else if (id == "manager") {
						while (true) {
							int choice;
							cout << endl;
							cout << "\t\tWELCOME STORAGE MANAGER" << endl;
							cout << "1. Add Storage" << endl;
							cout << "2. Personal Details" << endl;
							cout << "3. Storage Management" << endl;
							cout << "4. Prediction Model" << endl;
							cout << "5. Exit" << endl;
							cout << endl;
							cout << "Enter desired choice: ";
							cin >> choice;

							if (choice == 1) {
								cout << "\t\t ADDING/UPDATING STORAGE" << endl << endl;
								AddNode obj3;
								obj3.StorageAdd();
								cout << endl;

							}
							else if (choice == 2) {
								//personal detials
							}
							else if (choice == 3) {

							}
							else if (choice == 4) {
								//prediction model
							}
							else if (choice == 5) {
								break;
							}
						}
					}
					break;
				}
			}
			break;
		}
		else if (a == 2) {
			string id = obj.occupat();
			if (obj.registering()) {
				if (id == "farmer") {
					obj.registerfarmer();
				}
				else if (id == "manager") {
					obj.registermanager();
				}
			}

			
		}
		else if (a == 3) {
			break;
		}
	}
}