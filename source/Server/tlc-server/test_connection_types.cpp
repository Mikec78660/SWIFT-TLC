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
        
        driver = sql::mysql::get_mysql_driver_instance();
        
        // Test 1: Using localhost
        std::cout << "Test 1: Connecting to localhost..." << std::endl;
        conn = driver->connect("tcp://localhost:3306", "vsadmin", "vsadmin123");
        std::cout << "✓ localhost connection successful!" << std::endl;
        delete conn;
        
        // Test 2: Using 127.0.0.1
        std::cout << "\nTest 2: Connecting to 127.0.0.1..." << std::endl;
        conn = driver->connect("tcp://127.0.0.1:3306", "vsadmin", "vsadmin123");
        std::cout << "✓ 127.0.0.1 connection successful!" << std::endl;
        delete conn;
        
        // Test 3: Direct IP without protocol
        std::cout << "\nTest 3: Direct IP connection..." << std::endl;
        conn = driver->connect("127.0.0.1:3306", "vsadmin", "vsadmin123");
        std::cout << "✓ Direct IP connection successful!" << std::endl;
        
        sql::Statement* stmt = conn->createStatement();
        sql::ResultSet* res = stmt->executeQuery("SELECT @@hostname as hostname, @@bind_address as bind_addr");
        if (res->next()) {
            std::cout << "MySQL hostname: " << res->getString("hostname") << std::endl;
            std::cout << "MySQL bind address: " << res->getString("bind_addr") << std::endl;
        }
        
        delete res;
        delete stmt;
        delete conn;
        
        return 0;
        
    } catch (sql::SQLException& e) {
        std::cout << "❌ SQLException: " << e.what() << std::endl;
        std::cout << "SQLState: " << e.getSQLState() << std::endl;
        std::cout << "ErrorCode: " << e.getErrorCode() << std::endl;
        return 1;
    } catch (std::exception& e) {
        std::cout << "❌ Exception: " << e.what() << std::endl;
        return 1;
    }
}
