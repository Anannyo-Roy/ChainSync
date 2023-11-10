#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
using namespace std;


#include "MySQLDatabase.h"

MySQLDatabase::MySQLDatabase(const string& server, const string& username, const string& password, const string& database)
    : server_(server), username_(username), password_(password), database_(database) {
    driver_ = sql::mysql::get_mysql_driver_instance();
}

MySQLDatabase::MySQLDatabase() {};

bool MySQLDatabase::Connect() {
    try {
        con_ = driver_->connect(server_, username_, password_);
        con_->setSchema(database_);
        return true;
    }
    catch (sql::SQLException& e) {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
        return false;
    }
}

bool MySQLDatabase::RunQuery(string createTableQuery) {
    try {
        sql::Statement* stmt = con_->createStatement();
        stmt->execute(createTableQuery);
        return true;
    }
    catch (sql::SQLException e) {
        cerr << "In Error : " << e.what() << endl;
        return false;
    }

}

sql::ResultSet* MySQLDatabase::SelectData(string selectQuery) {
    if (!con_) {
        std::cerr << "Not connected to the database." << std::endl;
        return nullptr;
    }

    try {
        sql::Statement* stmt = con_->createStatement();
        sql::ResultSet* res = stmt->executeQuery(selectQuery);
        return res;
    }
    catch (sql::SQLException& e) {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
        return nullptr;
    }
}

//MySQLDatabase::~MySQLDatabase() {
//    if (con_) {
//        delete con_;
//    }
//}
//

