#ifndef CONNECTION_POOL_H
#define CONNECTION_POOL_H

#include "MySQLConnection.h"
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/consoleappender.h>
#include <log4cplus/layout.h>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/exception.h>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <atomic>
#include <vector>
#include <queue>
#include <functional>
#include <string>

namespace VS {
namespace Database {

/**
 * @brief Database connection pool configuration
 * 
 * Contains all configuration parameters for the connection pool.
 */
struct ConnectionPoolConfig {
    // Database connection parameters
    std::string host;
    unsigned int port;
    std::string database;
    std::string username;
    std::string password;
    
    // Pool size configuration
    size_t initialPoolSize = 5;           // Initial number of connections to create
    size_t maxPoolSize = 20;              // Maximum number of connections in pool
    size_t minPoolSize = 2;               // Minimum number of connections to maintain
    
    // Timeout configuration (in seconds)
    unsigned int connectionTimeout = 30;      // Timeout for creating new connections
    unsigned int idleTimeout = 300;           // Time before closing idle connections
    unsigned int maxLifetime = 3600;          // Maximum lifetime of a connection
    unsigned int validationTimeout = 5;       // Timeout for connection validation
    
    // Health check configuration
    bool enableHealthCheck = true;         // Enable periodic health checks
    unsigned int healthCheckInterval = 60; // Interval between health checks (seconds)
    unsigned int maxValidationRetries = 3; // Maximum validation retry attempts
    
    // Thread pool configuration for async operations
    size_t workerThreadCount = 2;          // Number of worker threads for async operations
    
    // Logging configuration
    std::string logLevel = "INFO";         // Logging level
    std::string logPattern = "%D{%Y-%m-%d %H:%M:%S.%q} [%t] %-5p %c{1}:%L - %m%n";
    
    // Validation query
    std::string validationQuery = "SELECT 1";
    
    // Connection retry configuration
    unsigned int maxRetries = 3;           // Maximum connection creation retries
    unsigned int retryDelay = 1;           // Delay between retries (seconds)
};

/**
 * @brief Statistics for connection pool monitoring (internal, with atomics)
 */
struct ConnectionPoolStats {
    std::atomic<size_t> totalConnections{0};        // Total connections created
    std::atomic<size_t> activeConnections{0};       // Currently active connections
    std::atomic<size_t> idleConnections{0};         // Currently idle connections
    std::atomic<size_t> failedConnections{0};       // Total failed connection attempts
    std::atomic<size_t> totalRequests{0};           // Total connection requests
    std::atomic<size_t> successfulRequests{0};      // Successful connection requests
    std::atomic<size_t> timedOutRequests{0};        // Timed out connection requests
    std::atomic<size_t> healthCheckFailures{0};     // Total health check failures
    std::chrono::steady_clock::time_point lastHealthCheck; // Last health check time
};

/**
 * @brief Statistics snapshot for external use (copyable, no atomics)
 */
struct ConnectionPoolStatsSnapshot {
    size_t totalConnections{0};        // Total connections created
    size_t activeConnections{0};       // Currently active connections
    size_t idleConnections{0};         // Currently idle connections
    size_t failedConnections{0};       // Total failed connection attempts
    size_t totalRequests{0};           // Total connection requests
    size_t successfulRequests{0};      // Successful connection requests
    size_t timedOutRequests{0};        // Timed out connection requests
    size_t healthCheckFailures{0};     // Total health check failures
    std::chrono::steady_clock::time_point lastHealthCheck; // Last health check time
    
    /**
     * @brief Get connection utilization percentage
     * @return Utilization percentage (0.0 to 1.0)
     */
    double getUtilization() const {
        size_t total = activeConnections + idleConnections;
        return total > 0 ? static_cast<double>(activeConnections) / total : 0.0;
    }
    
    /**
     * @brief Get success rate percentage
     * @return Success rate percentage (0.0 to 1.0)
     */
    double getSuccessRate() const {
        return totalRequests > 0 ? 
               static_cast<double>(successfulRequests) / totalRequests : 0.0;
    }
};

/**
 * @brief Database connection pool class
 * 
 * This class provides a thread-safe connection pool for MySQL databases.
 * It manages connection creation, health checks, timeout handling, and provides
 * both synchronous and asynchronous connection acquisition.
 */
class ConnectionPool {
public:
    /**
     * @brief Constructor
     * @param config Connection pool configuration
     */
    explicit ConnectionPool(const ConnectionPoolConfig& config);
    
    /**
     * @brief Destructor
     * Stops all background threads and cleans up connections
     */
    ~ConnectionPool();
    
    // Delete copy constructor and assignment operator
    ConnectionPool(const ConnectionPool&) = delete;
    ConnectionPool& operator=(const ConnectionPool&) = delete;
    
    // Enable move constructor and assignment
    ConnectionPool(ConnectionPool&& other) = delete;
    ConnectionPool& operator=(ConnectionPool&& other) = delete;
    
    /**
     * @brief Initialize the connection pool
     * Creates initial connections and starts background threads
     * @return true if initialization successful, false otherwise
     */
    bool initialize();
    
    /**
     * @brief Shutdown the connection pool
     * Stops background threads and closes all connections
     */
    void shutdown();
    
    /**
     * @brief Get a connection from the pool (synchronous)
     * @param timeout Maximum time to wait for a connection
     * @return MySQLConnection object or nullptr if timeout
     */
    std::unique_ptr<MySQLConnection> getConnection(
        std::chrono::seconds timeout = std::chrono::seconds(30));
    
    /**
     * @brief Get a connection from the pool (asynchronous)
     * @param callback Callback function to receive the connection
     * @param timeout Maximum time to wait for a connection
     */
    void getConnectionAsync(
        std::function<void(std::unique_ptr<MySQLConnection>)> callback,
        std::chrono::seconds timeout = std::chrono::seconds(30));
    
    /**
     * @brief Return a connection to the pool
     * @param connection Connection to return (will be moved into pool)
     */
    void returnConnection(std::unique_ptr<MySQLConnection> connection);
    
    /**
     * @brief Get current pool statistics
     * @return ConnectionPoolStatsSnapshot object containing current statistics
     */
    ConnectionPoolStatsSnapshot getStats() const;
    
    /**
     * @brief Force health check on all connections
     * @return Number of healthy connections found
     */
    size_t performHealthCheck();
    
    /**
     * @brief Add a new connection to the pool
     * @return true if connection added successfully, false otherwise
     */
    bool addConnection();
    
    /**
     * @brief Remove excess idle connections
     * Closes connections beyond the minimum pool size if they've been idle too long
     */
    void removeExcessConnections();
    
    /**
     * @brief Get pool configuration
     * @return Reference to the current configuration
     */
    const ConnectionPoolConfig& getConfig() const { return config_; }
    
    /**
     * @brief Check if the pool is healthy
     * @return true if pool is operational, false otherwise
     */
    bool isHealthy() const;
    
    /**
     * @brief Set a custom validation function
     * @param validator Function that takes a connection and returns true if valid
     */
    void setCustomValidator(std::function<bool(sql::Connection*)> validator) {
        customValidator_ = validator;
    }
    
    /**
     * @brief Set a custom connection factory
     * @param factory Function that creates new connections
     */
    void setCustomFactory(std::function<sql::Connection*()> factory) {
        customFactory_ = factory;
    }

private:
    // Configuration and logging
    ConnectionPoolConfig config_;
    log4cplus::Logger logger_;
    
    // Connection management
    std::queue<std::unique_ptr<MySQLConnection>> availableConnections_;
    std::vector<std::unique_ptr<MySQLConnection>> allConnections_;
    std::mutex poolMutex_;
    std::condition_variable poolCondition_;
    
    // Threading
    std::atomic<bool> isRunning_;
    std::vector<std::thread> workerThreads_;
    
    // Statistics
    mutable ConnectionPoolStats stats_;
    
    // Custom functions
    std::function<bool(sql::Connection*)> customValidator_;
    std::function<sql::Connection*()> customFactory_;
    
    // Background thread methods
    void healthCheckWorker();
    void connectionMonitorWorker();
    void asyncRequestWorker();
    
    // Connection management helpers
    sql::Connection* createNewConnection();
    bool validateConnection(sql::Connection* connection);
    void closeConnection(std::unique_ptr<MySQLConnection> connection);
    void cleanupIdleConnections();
    
    // Utility methods
    void logStats() const;
    void initializeLogging();
    std::string buildConnectionString() const;
};

} // namespace Database
} // namespace VS

#endif // CONNECTION_POOL_H
