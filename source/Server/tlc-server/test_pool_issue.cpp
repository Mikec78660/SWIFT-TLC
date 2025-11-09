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
        
        std::cout << "=== Testing vfsserver pool connection issue ===" << std::endl;
        
        // Test rapid connections like vfsserver ConnectionPool
        for (int i = 0; i < 10; i++) {
            try {
                std::cout << "Connection attempt " << (i+1) << "..." << std::endl;
                conn = driver->connect("tcp://127.0.0.1:3306", "vsadmin", "vsadmin123");
                
                sql::Statement* stmt = conn->createStatement();
                sql::ResultSet* res = stmt->executeQuery("SELECT 1 as test, NOW() as timestamp");
                
                if (res->next()) {
                    std::cout << "✓ Connection " << (i+1) << " successful: " << res->getInt("test") << std::endl;
                }
                
                delete res;
                delete stmt;
                delete conn;
                
            } catch (sql::SQLException& e) {
                std::cout << "❌ Connection " << (i+1) << " failed: " << e.what() << std::endl;
                std::cout << "   SQLState: " << e.getSQLState() << " | ErrorCode: " << e.getErrorCode() << std::endl;
                break; // Stop on first failure like vfsserver
            }
        }
        
        std::cout << "\n✅ Pool test completed" << std::endl;
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
