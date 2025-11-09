#ifndef MYSQL_CONNECTION_H
#define MYSQL_CONNECTION_H

#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/metadata.h>
#include <cppconn/exception.h>
#include <string>
#include <vector>
#include <memory>
#include <chrono>
#include <atomic>

namespace VS {
namespace Database {

/**
 * @brief MySQL connection wrapper class
 * 
 * This class wraps a MySQL connection and provides RAII-based resource management.
 * It tracks connection health, last used time, and provides basic database operations.
 */
class MySQLConnection {
public:
    /**
     * @brief Constructor
     * @param connection MySQL connection handle (managed externally)
     * @param id Unique connection identifier
     * @param createdAt Connection creation timestamp
     */
    MySQLConnection(sql::Connection* connection, int id, std::chrono::steady_clock::time_point createdAt);
    
    /**
     * @brief Destructor
     * Closes the MySQL connection if still open
     */
    ~MySQLConnection();
    
    // Delete copy constructor and assignment operator
    MySQLConnection(const MySQLConnection&) = delete;
    MySQLConnection& operator=(const MySQLConnection&) = delete;
    
    // Enable move constructor and assignment
    MySQLConnection(MySQLConnection&& other) noexcept;
    MySQLConnection& operator=(MySQLConnection&& other) noexcept;
    
    /**
     * @brief Execute a query
     * @param query SQL query string
     * @return true if successful, false otherwise
     */
    bool executeQuery(const std::string& query);
    
    /**
     * @brief Execute a query and return result set
     * @param query SQL query string
     * @return std::unique_ptr<sql::ResultSet> result set or nullptr on failure
     */
    std::unique_ptr<sql::ResultSet> executeQueryWithResult(const std::string& query);
    
    /**
     * @brief Execute a prepared statement
     * @param query SQL query with placeholders
     * @param params Vector of parameter values
     * @return true if successful, false otherwise
     */
    bool executePreparedQuery(const std::string& query, const std::vector<std::string>& params);
    
    /**
     * @brief Begin a transaction
     * @return true if successful, false otherwise
     */
    bool beginTransaction();
    
    /**
     * @brief Commit current transaction
     * @return true if successful, false otherwise
     */
    bool commit();
    
    /**
     * @brief Rollback current transaction
     * @return true if successful, false otherwise
     */
    bool rollback();
    
    /**
     * @brief Check if connection is healthy
     * @return true if connection is healthy, false otherwise
     */
    bool isHealthy() const;
    
    /**
     * @brief Get the MySQL connection handle
     * @return sql::Connection* pointer to the connection
     */
    sql::Connection* getConnection() const { return connection_; }
    
    /**
     * @brief Get connection ID
     * @return Connection ID
     */
    int getId() const { return id_; }
    
    /**
     * @brief Get connection creation time
     * @return Creation timestamp
     */
    std::chrono::steady_clock::time_point getCreatedAt() const { return createdAt_; }
    
    /**
     * @brief Get last used time
     * @return Last used timestamp
     */
    std::chrono::steady_clock::time_point getLastUsed() const { return lastUsed_; }
    
    /**
     * @brief Update last used time
     */
    void updateLastUsed();
    
    /**
     * @brief Get the number of seconds since last used
     * @return Seconds since last used
     */
    long long getSecondsSinceLastUsed() const;
    
    /**
     * @brief Get the number of seconds since creation
     * @return Seconds since creation
     */
    long long getSecondsSinceCreation() const;
    
    /**
     * @brief Mark connection as potentially invalid (for health checks)
     */
    void markAsPotentiallyInvalid() { isPotentiallyInvalid_ = true; }
    
    /**
     * @brief Reset the potentially invalid flag after successful health check
     */
    void resetPotentiallyInvalid() { isPotentiallyInvalid_ = false; }
    
    /**
     * @brief Check if connection is marked as potentially invalid
     * @return true if marked as potentially invalid
     */
    bool isPotentiallyInvalid() const { return isPotentiallyInvalid_; }

private:
    sql::Connection* connection_;
    int id_;
    std::chrono::steady_clock::time_point createdAt_;
    std::chrono::steady_clock::time_point lastUsed_;
    std::atomic<bool> isPotentiallyInvalid_;
};

} // namespace Database
} // namespace VS

#endif // MYSQL_CONNECTION_H
