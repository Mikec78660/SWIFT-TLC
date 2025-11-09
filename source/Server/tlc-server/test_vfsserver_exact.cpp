#include <iostream>
#include <mysql_driver.h>
#include <cppconn/connection.h>
#include <cppconn/exception.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/metadata.h>

int main() {
    try {
        sql::mysql::MySQL_Driver* driver;
        sql::Connection* conn;
        
        std::cout << "Testing exact vfsserver connection scenario..." << std::endl;
        std::cout << "Based on strace: connecting to 127.0.0.1:3306" << std::endl;
        
        driver = sql::mysql::get_mysql_driver_instance();
        
        // Test with exact parameters that vfsserver uses
        std::cout << "\nTest 1: Exact vfsserver connection (127.0.0.1:3306)..." << std::endl;
        conn = driver->connect("tcp://127.0.0.1:3306", "vsadmin", "vsadmin123");
        std::cout << "✓ vfsserver-style connection successful!" << std::endl;
        
        // Test connection pool simulation
        std::cout << "\nTest 2: Connection pool simulation..." << std::endl;
        sql::Statement* stmt = conn->createStatement();
        sql::ResultSet* res = stmt->executeQuery("SELECT 'ConnectionPool' as test_type, NOW() as timestamp");
        
        if (res->next()) {
            std::cout << "Query result: " << res->getString("test_type") << " - " << res->getString("timestamp") << std::endl;
        }
        
        // Test database selection
        std::cout << "\nTest 3: Database operations (like vfsserver would do)..." << std::endl;
        stmt->execute("USE vsdb");
        sql::ResultSet* res2 = stmt->executeQuery("SELECT COUNT(*) as table_count FROM information_schema.tables WHERE table_schema = 'vsdb'");
        
        if (res2->next()) {
            std::cout << "Tables in vsdb database: " << res2->getInt("table_count") << std::endl;
        }
        
        // Test connection metadata (this might reveal differences)
        std::cout << "\nTest 4: Connection metadata..." << std::endl;
        sql::DatabaseMetaData* meta = conn->getMetaData();
        std::cout << "Database product name: " << meta->getDatabaseProductName() << std::endl;
        std::cout << "Database product version: " << meta->getDatabaseProductVersion() << std::endl;
        std::cout << "Driver name: " << meta->getDriverName() << std::endl;
        std::cout << "Driver version: " << meta->getDriverVersion() << std::endl;
        
        delete res;
        delete res2;
        delete stmt;
        delete conn;
        
        std::cout << "\n✅ All vfsserver-style tests passed!" << std::endl;
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
