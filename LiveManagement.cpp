#include <iostream>
#include "MySQLDatabase.h"
#include "linear.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include "LiveManagement.h"

using namespace std;
using namespace sql;

//LiveManagement::LiveManagement() {}

LiveManagement::LiveManagement(string managerid, int storageMaxCapacity, int storageCapacity) {
    this->managerid = managerid;
    this->storageMaxCapacity = storageMaxCapacity;
    this->storageCapacity = storageCapacity;
    MySQLDatabase db("tcp://localhost:3306", "root", "root", "project");
    string q;
    q = "create table if not exists Farmertable( farmerid varchar(50), fpassword varchar(50), farmname varchar(50), farmloc varchar(50),farmsize double, fname varchar(50), fphone varchar(50), fgender varchar(20))";
    db.RunQuery(q);
    string q1 = "create table if not exists Managertable( managerid varchar(50), mpassword varchar(50), storagename varchar(50), storageloc varchar(50),storagemaxcapacity double, storagecapacity double, mname varchar(50), mphone varchar(50), mgender varchar(20))";
    db.RunQuery(q1);
    string cu = "create it not exists currentuser(id varchar(50), occ varchar(50))";
    db.RunQuery(cu);
    string f = "create it not exists tomato(season int,spoilagerate double,rainfall int)";
    db.RunQuery(f);
    string w = "select * from tomato";
    ResultSet* rs1 = db.SelectData(w);
    if (!rs1) {
        string in = "insert into tomato values(1,0.03,2),(2,0.05,2),(3,0.07,3),(4,0.02,1)";
        db.RunQuery(in);
    }
    string f1 = "create it not exists time(season int, rainfall int)";
    db.RunQuery(f1);
    string w1 = "select * from time";
    ResultSet* rs2 = db.SelectData(w1);
    if (!rs2) {
        string in1 = "insert into time values(3,3)";
        db.RunQuery(in1);
    }
}

string LiveManagement::getmanagerid() {
    MySQLDatabase db("tcp://localhost:3306", "root", "root", "project");
    if (db.Connect()) {
        try {
            string mid;
            string q = "select id from CurrentUser";
            ResultSet* rs = db.SelectData(q);
            if (rs) {
                while (rs->next()) {
                    mid = rs->getString("id");
                }
            }
            return mid;
        }
        catch (sql::SQLException& e) {
            cerr << "Sql Error : " << e.what() << endl;
        }
    }
}
void LiveManagement::setmanagerid(string managerid) {
    this->managerid = managerid;
}
int LiveManagement::getStorageMaxCapacity() {
    MySQLDatabase db("tcp://localhost:3306", "root", "root", "project");
    if (db.Connect()) {
        try {
            string a = "select storagemaxcapacity from managertable";
            ResultSet* rs = db.SelectData(a);
            if (rs) {
                while (rs->next()) {
                    return stoi(rs->getString("storagemaxcapacity"));
                }
            }
        }
        catch (sql::SQLException& e) {
            cerr << "Sql Error : " << e.what() << endl;
        }
    }
}
void LiveManagement::setStorageMaxCapacity(int storageMaxCapacity) {
    this->storageMaxCapacity = storageMaxCapacity;
}

int LiveManagement::getStorageCapacity() {
    MySQLDatabase db("tcp://localhost:3306", "root", "root", "project");
    if (db.Connect()) {
        try {
            string a = "select storagemaxcapacity from managertable";
            ResultSet* rs = db.SelectData(a);
            if (rs) {
                while (rs->next()) {
                    return stoi(rs->getString("storagemaxcapacity"));
                }
            }
        }
        catch (sql::SQLException& e) {
            cerr << "Sql Error : " << e.what() << endl;
        }
    }
}
void LiveManagement::setStorageCapacity(int storageCapacity) {
    this->storageCapacity = storageCapacity;
}

void LiveManagement::LMMenu(string managerid, int storageMaxCapacity, int storageCapacity) {
    MySQLDatabase db("tcp://localhost:3306", "root", "root", "project");

    if (db.Connect()) {
        try {
            linear lr;
            string currseason, currspoilagerate;
            string a1 = "select season from time";
            ResultSet* rs = db.SelectData(a1);
            if (rs) {
                while (rs->next()) {
                    currseason = rs->getString("season");
                }
            }
            string a2 = "select season, spoilage rate from tomato where season = " + currseason;
            ResultSet* rs0 = db.SelectData(a2);
            if (rs0) {
                while (rs0->next()) {
                    currspoilagerate = stoi(rs0->getString("spoilagerate"));
                }
            }

            int choice;
            cout << "Storage Manager Menu" << endl;
            cout << "1. Show storage max capacity" << endl;
            cout << "2. Show storage current capacity" << endl;
            cout << "3. Sell product" << endl;
            cout << "4. Exit" << endl;
            cout << "Enter your choice: ";
            cin >> choice;
            bool check = true;
            while (check) {
                int amount;
                cout << "Enter the amount you want to sell: ";
                cin >> amount;
                if (choice == 1) {
                    cout << "Storage max capacity: " << storageMaxCapacity << endl;
                }
                if (choice == 2) {
                    cout << "Storage current capacity: " << storageCapacity << endl;
                }
                if (choice == 3) {
                    /*Eigen::VectorXd input(5);
                    double predictedSP;
                    for (int i = 0; i < 5; i++) {
                        if (i == 0) {
                            input(i) = stoi(currseason);
                        }
                        else if (i == 1) {
                            input(i) = stod(currspoilagerate);
                        }
                        else if (i == 2) {
                            input(i) = 105;
                        }
                        else if (i == 3) {
                            input(i) = amount;
                        }
                        else if (i == 4) {
                            std::cout << "Enter quality (int): ";
                            std::cin >> input(4);
                        }
                    }*/
                    /*lr.trainModel();
                    lr.evaluateModel();
                    predictedSP = lr.predictSP(input);

                    cout << "Predicted selling price per kg is : " << predictedSP << endl;

                    cout << "Predicted selling price for given amount is : " << predictedSP*amount << endl;*/
                    int newamount = storageCapacity - amount;
                    string managing = "UPDATE storage SET storageCapacity = " + to_string(newamount) + " WHERE managerid = " + managerid;
                    db.RunQuery(managing);
                }
                if (choice == 4) {
                    cout << "exit" << endl;
                    break;
                }
                else {
                    cout << "Invalid choice" << endl;
                }
            }
        }
        catch (sql::SQLException& e) {
            cerr << "Sql Error : " << e.what() << endl;
        }
    }
}


//int main() {
//    string managerid = getmanagerid(); 
//    int storageMaxCapacity = getStorageMaxCapacity(); 
//    int storageCapacity = getStorageCapacity(); 
//    LiveManagement lm(managerid, storageMaxCapacity, storageCapacity); 
//    
//    
//    lm.LMMenu(managerid, storageMaxCapacity, storageCapacity);
//}