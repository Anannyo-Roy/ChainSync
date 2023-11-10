#include<iostream>
#include"MySQLDatabase.h"
#include"LoginSignup.h"
using namespace std;
using namespace sql;


    LoginSignup::LoginSignup() {
        db = MySQLDatabase("tcp://localhost:3306", "root", "1234", "projtest");
        if (db.Connect()) {
            try {
                string q;
                q = "create table if not exists Farmertable( farmerid varchar(50), fpassword varchar(50), farmname varchar(50), farmloc varchar(50),farmsize double, fname varchar(50), fphone varchar(50), fgender varchar(20))";
                db.RunQuery(q);
                q = "create table if not exists Managertable( managerid varchar(50), mpassword varchar(50), storagename varchar(50), storageloc varchar(50),storagemaxcapacity double, storagecapacity double, mname varchar(50), mphone varchar(50), mgender varchar(20))";
                db.RunQuery(q);
            }
            catch (sql::SQLException e) {
                cerr << "SQL error : " << e.what() << endl;
            }
        }
    }

    void LoginSignup::OccupationChoice() {            //WE GET THE CHOICE BACK, USE IT TO MATCH THE OPTIONS GIVEN, 1 for farmer and 2 for storage

        if (db.Connect()) {
            try {
                int choice;
                //creating a current user table
                string q;
                q = "create table if not exists CurrentUser(id varchar(50), occ varchar(30))";
                if (db.RunQuery(q)) {
                    q = "delete from currentuser";
                    db.RunQuery(q);
                    q = "insert into currentuser values(NULL,NULL)";
                    db.RunQuery(q);
                }

                //choice
                while (true) {
                    cout << "Continue As : " << endl << endl;
                    cout << "1. FARMER" << endl;
                    cout << "2. STORAGE ADMIN/ MANAGER" << endl;
                    cout << endl << endl;

                    cout << "Enter Choice : ";
                    cin >> choice;
                    if (choice == 1 || choice == 2) {
                        cout << endl;
                        break;
                    }
                    else {
                        cout << "Kindly Enter a valid Option!" << endl << endl;
                    }
                }
                if (choice == 1) {
                    q = "update currentuser set occ ='farmer'";
                    db.RunQuery(q);
                }
                else if (choice == 2) {
                    q = "update currentuser set occ ='manager'";
                    db.RunQuery(q);
                }
            }
            catch (sql::SQLException e) {
                cerr << "Sql error : " << e.what() << endl;
            }
        }
    }

    int LoginSignup::LoginRegisterChoice() {            //WE GET THE CHOICE BACK, USE IT TO MATCH THE OPTIONS GIVEN, 1 for login and 2 for register and 3 for back

        if (db.Connect()) {
            try {

                string q;
                q = "update currentuser set id = NULL";
                db.RunQuery(q);

                int choice;
                //choice
                while (true) {
                    cout << "Welcome, Would you like to : " << endl << endl;
                    cout << "1. LOGIN" << endl;
                    cout << "2. REGISTER" << endl;
                    cout << "3. Back" << endl;
                    cout << endl << endl;

                    cout << "Enter Choice : ";
                    cin >> choice;
                    if (choice == 1 || choice == 2 || choice == 3) {
                        break;
                    }
                    else {
                        cout << "Kindly enter a valid option!" << endl;
                    }
                }
                return choice;
            }
            catch (sql::SQLException e) {
                cerr << "Sql error : " << e.what() << endl;
            }
        }
    }

    string LoginSignup::occupat() {
        if (db.Connect()) {
            try {
                string q;
                string occupation;
                q = "select occ from currentuser";
                ResultSet* rs = db.SelectData(q);
                if (rs) {
                    while (rs->next()) {
                        occupation = rs->getString("occ");
                    }
                }
                return occupation;
            }
            catch (sql::SQLException e) {
                cerr << "SQL error : " << e.what() << endl;
            }
        }
    }

    string LoginSignup::currid() {
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

    bool LoginSignup::login() {   //from login we use it in if to check if successful or not, Then we check the occupation and redirect it to that occupations dashboard

        if (db.Connect()) {

            try {
                string q;
                q = "update currentuser set id = NULL";
                db.RunQuery(q);

                string occupation = occupat();

                string name;
                string pass;

                //main input

                cout << "LOGIN USER" << endl << endl;
                cout << "Enter Username : ";
                cin >> name;
                cout << "Enter Password : ";
                cin >> pass;

                if (occupation == "farmer") {
                    q = "select * from FarmerTable where farmerid ='" + name + "' and fpassword ='" + pass + "'";
                    ResultSet* rs1 = db.SelectData(q);
                    if (rs1->next()) {
                        //adding to currentuser
                        q = "update currentuser set id ='" + name + "'";
                        db.RunQuery(q);
                        return true;
                    }
                    else {
                        return false;
                    }
                }
                else if (occupation == "manager") {
                    q = "select * from ManagerTable where managerid ='" + name + "' and mpassword ='" + pass + "'";
                    ResultSet* rs2 = db.SelectData(q);
                    if (rs2->next()) {
                        //adding to currentuser
                        q = "update currentuser set id ='" + name + "'";
                        db.RunQuery(q);
                        return true;
                    }
                    else {
                        return false;
                    }
                }



            }
            catch (sql::SQLException e) {
                cerr << "Sql Error : " << e.what() << endl;
            }

        }
    }

    bool LoginSignup::isStrongPassword(string password) {
        if (password.length() < 5) {
            return false;
        }

        bool hasUpperCase = false;
        bool hasLowerCase = false;
        bool hasDigit = false;

        for (char c : password) {
            if (isupper(c)) {
                hasUpperCase = true;
            }
            else if (islower(c)) {
                hasLowerCase = true;
            }
            else if (isdigit(c)) {
                hasDigit = true;
            }
        }

        return hasUpperCase && hasLowerCase && hasDigit;
    }

    bool LoginSignup::registering() {  //if register is chosen 
        if (db.Connect()) {
            try {
                string q;
                

                string occ = occupat();

                string name, pass, cpass;
                int flag = 0;


                cout << "Please fill the following details : " << endl << endl;
                cout << "Username : ";
                cin >> name;
                cout << "Password (Atleast 5 letter long with digits and lower and uppercase letters): ";
                cin >> pass;
                cout << "Confirm Password : ";
                cin >> cpass;

                //check validity of username
                q = "select farmerid from farmertable";
                ResultSet* rs = db.SelectData(q);

                if (rs) {
                    while (rs->next()) {
                        if (rs->getString("farmerid") == name) {
                            flag = 1;
                        }
                    }
                }
                q = "select managerid from managertable";
                ResultSet* rs1 = db.SelectData(q);

                if (rs) {
                    while (rs1->next()) {
                        if (rs1->getString("managerid") == name) {
                            flag = 1;
                        }
                    }
                }

                //checking pass and occupation
                if (pass == cpass && isStrongPassword(pass) && occ == "farmer" && flag == 0) {
                    q = "insert into farmertable values('" + name + "','" + pass + "',NULL,NULL,NULL,NULL,NULL,NULL)";
                    db.RunQuery(q);
                    q = "update currentuser set id ='" + name + "'";
                    db.RunQuery(q);
                    return true;                //if true we can check occupation and then go back to login page
                }
                else if (pass == cpass && isStrongPassword(pass) && occ == "manager" && flag == 0) {
                    q = "insert into managertable values('" + name + "','" + pass + "',NULL,NULL,NULL,NULL,NULL,NULL,NULL)";
                    db.RunQuery(q);
                    q = "update currentuser set id ='" + name + "'";
                    db.RunQuery(q);
                    return true;
                }
                else {
                    cout << endl;
                    if (flag == 1) {
                        cout << "Username Already Taken, Please Try Again !" << endl;
                    }
                    else {
                        cout << "Invalid Password, Please Try Again !" << endl;
                    }
                    return false;                                       // goes all the way back to choosing occupation or login
                }

            }
            catch (sql::SQLException e) {
                cerr << "Sql error : " << e.what() << endl;
            }
        }
    }

    bool LoginSignup::registerfarmer() {
        if (db.Connect()) {
            try {
                string q, name, phone, gender;
                int flag = 0;
                int count = 0;
                string id = currid();

                while (true) {
                    cout << "Enter the Following details : " << endl << endl;
                    cout << "Name : ";
                    cin >> name;
                    cout << "Phone no. : ";
                    cin >> phone;
                    cout << "Gender : ";
                    cin >> gender;

                    for (char i : phone) {
                        if (!isdigit(i)) {
                            flag = 1;
                        }
                        count++;
                    }
                    if (count < 10 || count>10) {
                        flag = 1;
                    }

                    if (flag == 1) {
                        cout << "Please check and enter valid details" << endl;

                    }
                    else {
                        q = "update farmertable set fname='" + name + "' where farmerid ='" + id + "'";
                        db.RunQuery(q);
                        q = "update farmertable set fphone='" + phone + "' where farmerid ='" + id + "'";
                        db.RunQuery(q);
                        q = "update farmertable set fgender='" + gender + "' where farmerid ='" + id + "'";
                        db.RunQuery(q);
                        return true;
                    }
                }

            }
            catch (sql::SQLException e) {
                cerr << "SQL error : " << e.what() << endl;
            }
        }
    }

    bool LoginSignup::registermanager() {
        if (db.Connect()) {
            try {
                string q, name, phone, gender;
                int flag = 0;
                int count = 0;
                string id = currid();

                while (true) {
                    cout << "Enter the Following details : " << endl << endl;
                    cout << "Name : ";
                    cin >> name;
                    cout << "Phone no. : ";
                    cin >> phone;
                    cout << "Gender : ";
                    cin >> gender;

                    for (char i : phone) {
                        if (!isdigit(i)) {
                            flag = 1;
                            cout << "YES";
                        }
                        count++;
                    }
                    if (count < 10 || count>10) {
                        flag = 1;
                        cout << "NONONON";
                    }

                    if (flag == 1) {
                        cout << "Please check and enter valid details" << endl;

                    }
                    else {
                        q = "update managertable set mname='" + name + "' where managerid ='" + id + "'";
                        db.RunQuery(q);
                        q = "update managertable set mphone='" + phone + "' where managerid ='" + id + "'";
                        db.RunQuery(q);
                        q = "update managertable set mgender='" + gender + "' where managerid ='" + id + "'";
                        db.RunQuery(q);
                        return true;
                    }
                }

            }
            catch (sql::SQLException e) {
                cerr << "SQL error : " << e.what() << endl;
            }
        }
    }


   
