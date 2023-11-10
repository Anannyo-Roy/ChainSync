#ifndef MySqlDatabase_h
#define MySqlDatabase_h

#include <string>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/resultset.h>


using namespace std;

class MySQLDatabase {
public:
    MySQLDatabase();
    MySQLDatabase(const string& server, const string& username, const string& password, const string& database);

    bool Connect();
    bool RunQuery(string createTableQuery);
    sql::ResultSet* SelectData(string selectQuery);
    //~MySQLDatabase();

private:
    sql::mysql::MySQL_Driver* driver_;
    sql::Connection* con_;
    std::string server_;
    std::string username_;
    std::string password_;
    std::string database_;
};

#endif