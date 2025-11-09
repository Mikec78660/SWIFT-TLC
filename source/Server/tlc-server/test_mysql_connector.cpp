#include <iostream>
#include <mysql_driver.h>
#include <cppconn/connection.h>
#include <cppconn/exception.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>

int main() {
    try {
        sql::mysql::MySQL_Driver* driver;
        sql::Connection* conn;
        
        std::cout << "Testing MySQL Connector/C++ connection..." << std::endl;
        
        driver = sql::mysql::get_mysql_driver_instance();
        conn = driver->connect("tcp://localhost:3306", "vsadmin", "vsadmin123");
        
        std::cout << "Connection successful!" << std::endl;
        
        sql::Statement* stmt = conn->createStatement();
        sql::ResultSet* res = stmt->executeQuery("SELECT VERSION() as version");
        
        if (res->next()) {
            std::cout << "Database version: " << res->getString("version") << std::endl;
        }
        
        delete res;
        delete stmt;
        delete conn;
        
        return 0;
    } catch (sql::SQLException& e) {
        std::cout << "SQLException: " << e.what() << std::endl;
        std::cout << "SQLState: " << e.getSQLState() << std::endl;
        std::cout << "ErrorCode: " << e.getErrorCode() << std::endl;
        return 1;
    } catch (std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
        return 1;
    }
}
