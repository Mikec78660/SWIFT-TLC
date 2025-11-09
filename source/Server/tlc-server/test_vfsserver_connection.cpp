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
        
        std::cout << "Testing vfsserver-style connection..." << std::endl;
        std::cout << "Host: tcp://localhost:3306" << std::endl;
        std::cout << "Database: vsdb" << std::endl;
        std::cout << "User: vsadmin" << std::endl;
        
        driver = sql::mysql::get_mysql_driver_instance();
        
        // Test 1: Connection without database
        std::cout << "\nTest 1: Connect to MySQL without selecting database..." << std::endl;
        conn = driver->connect("tcp://localhost:3306", "vsadmin", "vsadmin123");
        std::cout << "✓ Connection successful (no DB)" << std::endl;
        
        sql::Statement* stmt1 = conn->createStatement();
        sql::ResultSet* res1 = stmt1->executeQuery("SELECT VERSION() as version");
        if (res1->next()) {
            std::cout << "Database version: " << res1->getString("version") << std::endl;
        }
        
        std::cout << "\nTest 2: Select database and list tables..." << std::endl;
        stmt1->execute("USE vsdb");
        sql::ResultSet* res2 = stmt1->executeQuery("SHOW TABLES");
        int tableCount = 0;
        while (res2->next()) {
            tableCount++;
            std::cout << "  Table " << tableCount << ": " << res2->getString(1) << std::endl;
        }
        
        // Test 3: Connection with database specified
        std::cout << "\nTest 3: Connect directly to vsdb database..." << std::endl;
        sql::Connection* conn2 = driver->connect("tcp://localhost:3306/vsdb", "vsadmin", "vsadmin123");
        std::cout << "✓ Direct connection to vsdb successful!" << std::endl;
        
        sql::Statement* stmt2 = conn2->createStatement();
        sql::ResultSet* res3 = stmt2->executeQuery("SELECT COUNT(*) as table_count FROM information_schema.tables WHERE table_schema = 'vsdb'");
        if (res3->next()) {
            std::cout << "Table count in vsdb: " << res3->getInt("table_count") << std::endl;
        }
        
        // Test 4: Test connection pool settings
        std::cout << "\nTest 4: Connection settings verification..." << std::endl;
        sql::ResultSet* res4 = stmt2->executeQuery("SELECT @@max_connections, @@connect_timeout, @@read_timeout");
        if (res4->next()) {
            std::cout << "Max connections: " << res4->getInt(1) << std::endl;
            std::cout << "Connect timeout: " << res4->getInt(2) << std::endl;
            std::cout << "Read timeout: " << res4->getInt(3) << std::endl;
        }
        
        delete res1;
        delete res2;
        delete res3;
        delete res4;
        delete stmt1;
        delete stmt2;
        delete conn;
        delete conn2;
        
        std::cout << "\n✅ All connection tests passed!" << std::endl;
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
