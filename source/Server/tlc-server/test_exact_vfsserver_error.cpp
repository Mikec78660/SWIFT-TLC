#include <iostream>
#include <mysql_driver.h>
#include <cppconn/connection.h>
#include <cppconn/exception.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/warning.h>
#include <cppconn/metadata.h>

int main() {
    try {
        sql::mysql::MySQL_Driver* driver;
        sql::Connection* conn;
        
        driver = sql::mysql::get_mysql_driver_instance();
        
        std::cout << "=== Testing vfsserver connection scenarios ===" << std::endl;
        
        // Test 1: Default connection
        std::cout << "\nTest 1: Standard connection..." << std::endl;
        conn = driver->connect("tcp://127.0.0.1:3306", "vsadmin", "vsadmin123");
        std::cout << "✓ Standard connection successful" << std::endl;
        
        // Test 2: Connection with warnings enabled
        std::cout << "\nTest 2: Connection with detailed error handling..." << std::endl;
        sql::Statement* stmt = conn->createStatement();
        sql::ResultSet* res = stmt->executeQuery("SELECT 1 as test");
        if (res->next()) {
            std::cout << "Basic query result: " << res->getInt("test") << std::endl;
        }
        
        // Test 3: Check for warnings
        std::cout << "\nTest 3: Checking for connection warnings..." << std::endl;
        sql::Warning* warning = conn->getWarnings();
        int warningCount = 0;
        while (warning) {
            std::cout << "Warning " << ++warningCount << ": " << warning->getMessage() << std::endl;
            warning = warning->getNextWarning();
        }
        if (warningCount == 0) {
            std::cout << "No warnings found" << std::endl;
        }
        
        // Test 4: Test with different connection string variations that might be used by vfsserver
        std::cout << "\nTest 4: Testing connection pool scenarios..." << std::endl;
        delete res;
        delete stmt;
        delete conn;
        
        // Test 5: Multiple rapid connections (simulating connection pool)
        for (int i = 0; i < 5; i++) {
            try {
                conn = driver->connect("tcp://127.0.0.1:3306", "vsadmin", "vsadmin123");
                sql::Statement* stmt2 = conn->createStatement();
                sql::ResultSet* res2 = stmt2->executeQuery("SELECT 1");
                if (res2->next()) {
                    std::cout << "Connection " << i+1 << " successful" << std::endl;
                }
                delete res2;
                delete stmt2;
                delete conn;
                
                // Brief pause between connections
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                
            } catch (sql::SQLException& e) {
                std::cout << "Connection " << i+1 << " failed: " << e.what() << " (SQLState: " << e.getSQLState() << ")" << std::endl;
            }
        }
        
        std::cout << "\n✅ Connection test completed" << std::endl;
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
