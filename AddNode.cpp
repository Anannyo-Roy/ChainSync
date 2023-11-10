#include<iostream>
#include<vector>
#include<sstream>
#include"MySQLDatabase.h"
#include"AddNode.h"
using namespace std;
using namespace sql;

AddNode::AddNode() {
	db = MySQLDatabase("tcp://localhost:3306", "root", "1234", "projtest");


	if (db.Connect()) {
		try {
			string q;
			q = "create table if not exists locations(loc varchar(50), name varchar(50))";
			db.RunQuery(q);
			q = "select * from locations";
			ResultSet* rs10 = db.SelectData(q);
			if (!rs10->next()) {
				q = "insert into locations values('0,8,0,11,0,0,0,0,0,0', 'Aarohan Transport Hub'), ('0,0,3,2,0,0,0,0,0,0', 'Bandra Distributors'), ('0,0,0,0,4,6,0,0,0,0', 'Columbia Center'), ('7,0,0,0,7,0,4,0,0,0', 'District Bus Stand'), ('0,5,6,0,0,0,0,1,9,0', 'Emporium Marg'), ('0,0,5,0,0,0,0,0,4,0', 'Fishery'), ('0,0,0,0,6,0,0,6,0,0', 'Green Packing House'), ('0,0,0,0,3,0,4,0,0,0', 'High Court Street'), ('0,0,0,0,0,1,0,0,0,0', 'Indo Grading Center')";
				db.RunQuery(q);
			}
		}
		catch (sql::SQLException e) {
			cerr << "Sql error : " << e.what() << endl;
		}
	}

	getInter();
}

void AddNode::getInter() {
	if (db.Connect()) {
		try {
			string q;
			q = "select name from locations limit 9";
			ResultSet* rs = db.SelectData(q);
			if (rs) {
				while (rs->next()) {
					intermerdiates.push_back(rs->getString("name"));
				}
			}
			/*for (string i : intermerdiates) {
				cout << i << endl;
			}*/
		}
		catch (sql::SQLException e) {
			cerr << "SQL error : " << e.what() << endl;
		}
	}
}

string AddNode::vectorToString(const vector<int>& numbers) {
	string result;
	for (size_t i = 0; i < numbers.size(); i++) {
		result += to_string(numbers[i]);
		if (i < numbers.size() - 1) {
			result += ",";
		}
	}
	return result;
}

vector<int> AddNode::stringToVector(const string& input) {
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

string AddNode::currid() {
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

void AddNode::FarmAdd() {
	string q;

	string id = currid();

	vector<int> temp(9, 0);
	cout << "Choose 2 Locations from which you are closest to : " << endl << endl;
	int maincount = 1;
	int count = 0;
	for (string i : intermerdiates) {
		cout << ++count << ". " << i << endl;
	}
	cout << endl;
	while (maincount < 3) {

		int choice;
		int flag = 0;
		string dist;

		cout << "Select location " << maincount << " : ";
		cin >> choice;
		if (choice > 0 && choice < count + 1) {

			cout << "Enter Distance (in KM) from your Farm to Location : ";
			cin >> dist;
			cout << endl;
			for (char i : dist) {
				if (!isdigit(i)) {
					flag = 1;
				}
			}
			if (flag == 1) {
				cout << "Provide valid Distance!" << endl;
			}
			else {
				maincount++;
				int intdist = stoi(dist);
				temp[choice - 1] = intdist;
			}

		}
		else {
			cout << "Choose a valid option" << endl;
		}

	}

	string done = vectorToString(temp);
	//cout << done;
	if (db.Connect()) {
		try {
			q = "update farmertable set farmloc='" + done + "' where farmerid ='" + id + "'";
			db.RunQuery(q);
			//
			cout << endl;
			farmname();
			cout << endl;
			farmsize();
			//
			cout << "Farm Added Successfully " << endl << endl;
		}
		catch (sql::SQLException e) {
			cerr << "SQL error : " << e.what() << endl;
		}
	}
	/*for (int i : temp) {
		cout << i << endl;
	}*/
}
void AddNode::farmname() {
	string name;
	cout << "Enter Farm Name : ";
	cin >> name;
	string q;
	string id = currid();

	if (db.Connect()) {
		try {
			q = "update farmertable set farmname ='" + name + "' where farmerid = '" + id + "'";
			db.RunQuery(q);
			cout << endl;
		}
		catch (sql::SQLException e) {
			cerr << "SQL error : " << e.what() << endl;
		}
	}
}

void AddNode::farmsize() {
	string size;
	int flag = 0;
	cout << "Enter Farm Size (In Hectare) : ";
	cin >> size;
	for (char i : size) {
		if (!isdigit(i)) {
			flag = 1;
		}
	}
	if (flag == 1) {
		cout << "Provide valid Distance!" << endl;
	}
	else {

		string q;
		string id = currid();

		if (db.Connect()) {
			try {
				q = "update farmertable set farmsize =" + size + " where farmerid = '" + id + "'";
				db.RunQuery(q);
				cout << endl;
			}
			catch (sql::SQLException e) {
				cerr << "SQL error : " << e.what() << endl;
			}
		}
	}
}


void AddNode::storagename() {
	string name;
	cout << "Enter Storage Name : ";
	cin >> name;
	cout << endl;
	string q;
	string id = currid();

	if (db.Connect()) {
		try {
			q = "update managertable set storagename ='" + name + "' where managerid = '" + id + "'";
			db.RunQuery(q);
		}
		catch (sql::SQLException e) {
			cerr << "SQL error : " << e.what() << endl;
		}
	}
}

void AddNode::storagecapacity() {   //implement before the capcity function
	string name;
	cout << "Enter Storage Capacity : ";
	cin >> name;
	cout << endl;
	string q;
	string id = currid();

	if (db.Connect()) {
		try {
			q = "select storagemaxcapacity from managertable where managerid='" + id + "'";
			ResultSet* rs = db.SelectData(q);
			int value;
			if (rs) {
				while (rs->next()) {
					value = rs->getDouble("storagemaxcapacity");
				}
			}
			if (value > stoi(name)) {
				q = "update managertable set storagecapacity =" + name + " where managerid = '" + id + "'";
				db.RunQuery(q);
			}
			else {
				cout << "Capacity cannot exceed the maximum capacity";
			}
		}
		catch (sql::SQLException e) {
			cerr << "SQL error : " << e.what() << endl;
		}
	}
}

void AddNode::storagemaxcapacity() {
	string name;
	cout << "Enter Maximum Storage Capacity : ";
	cin >> name;
	cout << endl;
	string q;
	string id = currid();

	if (db.Connect()) {
		try {
			q = "update managertable set storagemaxcapacity =" + name + " where managerid = '" + id + "'";
			db.RunQuery(q);
		}
		catch (sql::SQLException e) {
			cerr << "SQL error : " << e.what() << endl;
		}
	}
}

//Now for Clients
void AddNode::StorageAdd() {
	string id = currid();
	vector<int> temp(9, 0);

	storagename();
	storagemaxcapacity();
	storagecapacity();

	cout << "Choose 2 Locations from which you are closest to : " << endl << endl;
	int maincount = 1;
	int count = 0;
	for (string i : intermerdiates) {
		cout << ++count << ". " << i << endl;
	}
	cout << endl;
	while (maincount < 3) {

		int choice;
		int flag = 0;
		string dist;

		cout << "Select location " << maincount << " : ";
		cin >> choice;
		if (choice > 0 && choice < count + 1) {

			cout << "Enter Distance (in KM) from your Farm to Location : ";
			cin >> dist;
			cout << endl;
			for (char i : dist) {
				if (!isdigit(i)) {
					flag = 1;
				}
			}
			if (flag == 1) {
				cout << "Provide valid Distance!" << endl;
			}
			else {
				maincount++;
				int intdist = stoi(dist);
				temp[choice - 1] = intdist;
			}

		}
		else {
			cout << "Choose a valid option" << endl;
		}

	}
	string q;
	if (db.Connect()) {
		try {
			vector<vector<int>> graphlocations;
			vector<string> loc;
			q = "select loc from locations LIMIT 9";
			ResultSet* rs = db.SelectData(q);
			if (rs) {
				while (rs->next()) {
					loc.push_back(rs->getString("loc"));

				}
			}

			for (string i : loc) {
				vector<int> temp1 = stringToVector(i);
				graphlocations.push_back(temp1);
			}

			for (int i = 0; i < temp.size(); i++) {
				graphlocations[i].push_back(temp[i]);
			}
			int namecount = -1;
			for (vector<int> i : graphlocations) {
				string a = vectorToString(i);
				q = "update locations set loc ='" + a + "' where name ='" + intermerdiates[++namecount] + "'";
				db.RunQuery(q);
			}


			string storagename;
			q = "select storagename from managertable where managerid='" + id + "'";
			ResultSet* rs1 = db.SelectData(q);
			if (rs1) {
				while (rs1->next()) {
					storagename = rs1->getString("storagename");
				}
			}


			q = "select storage name from managertable where managerid='" + id + "'";
			ResultSet* rs12 = db.SelectData(q);
			//if (rs12->next()) {
			//	vector<int> residue(graphlocations[0].size(), 0);
			//	string another = vectorToString(residue);
			//	q = "update locations set name ='" + storagename + "' where loc ='" + another + "'";
			//}
			//else {
				vector<int> residue(graphlocations[0].size(), 0);
				string another = vectorToString(residue);
				q = "insert into locations values('" + another + "','" + storagename + "')";
				db.RunQuery(q);
			//}
			

			//adding storage location to manager table
			string x = vectorToString(temp);
			q = "update managertable set storageloc='" + x + "' where managerid='" + id + "'";
			db.RunQuery(q);


		}
		catch (sql::SQLException e) {
			cerr << "Sql error : " << e.what() << endl;
		}

	}
}



/*for (int i : temp) {
	cout << i << " ";
}*/






