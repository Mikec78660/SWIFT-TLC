# VS Database Connection Pool

A comprehensive, production-ready database connection pool implementation for MySQL databases using MySQL Connector/C++.

## Features

### Core Functionality
- **Thread-Safe Operations**: All connection pool operations are thread-safe using mutexes and condition variables
- **Connection Lifecycle Management**: Automatic creation, validation, and cleanup of database connections
- **Health Monitoring**: Periodic health checks and connection validation
- **Resource Management**: Configurable connection timeouts, idle timeouts, and maximum lifetimes
- **Statistics Tracking**: Comprehensive statistics for monitoring pool performance
- **Async Support**: Asynchronous connection acquisition using worker threads

### Advanced Features
- **Custom Validation**: Support for custom connection validation functions
- **Custom Factory**: Support for custom connection creation logic
- **Dynamic Pool Sizing**: Automatic scaling based on demand within configured limits
- **Graceful Shutdown**: Proper cleanup of resources during shutdown
- **Error Recovery**: Automatic retry logic and connection recovery
- **Logging Integration**: Comprehensive logging using log4cplus

## Architecture

### Class Structure

```
VS::Database
├── MySQLConnection       - Wrapper for individual MySQL connections
├── ConnectionPool        - Main connection pool management
├── ConnectionPoolConfig  - Configuration structure
└── ConnectionPoolStats   - Statistics and monitoring
```

### Key Components

#### MySQLConnection
- Wraps MySQL Connector/C++ connection objects
- Provides RAII-based resource management
- Tracks connection health and usage statistics
- Supports prepared statements and transactions
- Automatic cleanup on destruction

#### ConnectionPool
- Manages a pool of MySQLConnection objects
- Handles connection creation, validation, and cleanup
- Provides both synchronous and asynchronous access
- Maintains background worker threads for health checks
- Implements configurable timeout and retry logic

#### ConnectionPoolConfig
- Centralized configuration for all pool parameters
- Database connection parameters (host, port, database, credentials)
- Pool size limits (initial, min, max)
- Timeout configurations
- Health check settings
- Thread pool configuration

#### ConnectionPoolStats
- Real-time statistics tracking
- Atomic counters for thread-safe updates
- Performance metrics calculation
- Health monitoring data

## Configuration

### Basic Configuration

```cpp
VS::Database::ConnectionPoolConfig config;
config.host = "localhost";
config.port = 3306;
config.database = "mydb";
config.username = "user";
config.password = "password";
config.initialPoolSize = 5;      // Initial connections
config.maxPoolSize = 20;         // Maximum connections
config.minPoolSize = 2;          // Minimum connections to maintain
config.idleTimeout = 300;        // 5 minutes
config.maxLifetime = 3600;       // 1 hour
config.enableHealthCheck = true;
config.healthCheckInterval = 60; // 60 seconds
config.workerThreadCount = 2;
```

### Advanced Configuration

```cpp
config.connectionTimeout = 30;   // Connection creation timeout
config.validationTimeout = 5;    // Validation query timeout
config.maxValidationRetries = 3; // Maximum validation retry attempts
config.maxRetries = 3;           // Connection creation retries
config.retryDelay = 1;           // Delay between retries
config.validationQuery = "SELECT 1"; // Custom validation query
config.logLevel = "INFO";        // Logging level
config.logPattern = "%D{%Y-%m-%d %H:%M:%S.%q} [%t] %-5p %c{1}:%L - %m%n";
```

## Usage Examples

### Basic Usage

```cpp
#include "ConnectionPool.h"

int main() {
    // Configure the pool
    VS::Database::ConnectionPoolConfig config;
    config.host = "localhost";
    config.port = 3306;
    config.database = "test";
    config.username = "user";
    config.password = "pass";
    config.initialPoolSize = 3;
    config.maxPoolSize = 10;
    
    // Create and initialize the pool
    VS::Database::ConnectionPool pool(config);
    if (!pool.initialize()) {
        std::cerr << "Failed to initialize pool" << std::endl;
        return 1;
    }
    
    // Get a connection
    auto connection = pool.getConnection();
    if (connection) {
        // Use the connection
        connection->executeQuery("SELECT * FROM users");
        
        // Connection is automatically returned when it goes out of scope
    }
    
    // Shutdown the pool
    pool.shutdown();
    return 0;
}
```

### Concurrent Usage

```cpp
std::vector<std::thread> threads;
for (int i = 0; i < 10; ++i) {
    threads.emplace_back([&pool]() {
        for (int j = 0; j < 100; ++j) {
            auto connection = pool.getConnection(std::chrono::seconds(5));
            if (connection) {
                // Use the connection
                connection->executeQuery("SELECT 1");
                // Automatic return when connection goes out of scope
            }
        }
    });
}

for (auto& thread : threads) {
    thread.join();
}
```

### Async Usage

```cpp
pool.getConnectionAsync([](std::unique_ptr<VS::Database::MySQLConnection> connection) {
    if (connection) {
        // Use the connection asynchronously
        connection->executeQuery("SELECT * FROM data");
    }
}, std::chrono::seconds(5));
```

### Custom Validation

```cpp
pool.setCustomValidator([](sql::Connection* conn) -> bool {
    try {
        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        std::unique_ptr<sql::ResultSet> result(stmt->executeQuery("SELECT 1"));
        return result && result->next();
    } catch (const sql::SQLException& e) {
        return false;
    }
});
```

### Monitoring and Statistics

```cpp
// Get current statistics
auto stats = pool.getStats();
std::cout << "Total connections: " << stats.totalConnections << std::endl;
std::cout << "Active connections: " << stats.activeConnections << std::endl;
std::cout << "Utilization: " << (stats.getUtilization() * 100) << "%" << std::endl;
std::cout << "Success rate: " << (stats.getSuccessRate() * 100) << "%" << std::endl;

// Force a health check
size_t healthyConnections = pool.performHealthCheck();
std::cout << "Healthy connections: " << healthyConnections << std::endl;

// Check pool health
if (pool.isHealthy()) {
    std::cout << "Pool is healthy" << std::endl;
}
```

## Compilation Requirements

### Dependencies
- MySQL Connector/C++ (libmysqlcppconn)
- log4cplus
- C++11 or later
- pthread

### Compiler Flags
```bash
g++ -std=c++11 -I/path/to/mysql/connector/include \
    -L/path/to/mysql/connector/lib -lmysqlcppconn \
    -llog4cplus -lpthread -o your_program your_source.cpp
```

### CMake Integration
```cmake
find_package(MySQLCpp REQUIRED)
find_package(log4cplus REQUIRED)

add_executable(your_program your_source.cpp ConnectionPoolExample.cpp)
target_link_libraries(your_program MySQLCpp::mysqlcppconn log4cplus::log4cplus)
```

## API Reference

### MySQLConnection Class

#### Methods
- `MySQLConnection(sql::Connection* connection, int id, time_point createdAt)` - Constructor
- `~MySQLConnection()` - Destructor
- `bool executeQuery(const std::string& query)` - Execute SQL query
- `std::unique_ptr<sql::ResultSet> executeQueryWithResult(const std::string& query)` - Execute query with results
- `bool executePreparedQuery(const std::string& query, const std::vector<std::string>& params)` - Execute prepared statement
- `bool beginTransaction()` - Begin transaction
- `bool commit()` - Commit transaction
- `bool rollback()` - Rollback transaction
- `bool isHealthy() const` - Check connection health
- `sql::Connection* getConnection() const` - Get underlying connection
- `int getId() const` - Get connection ID
- `void updateLastUsed()` - Update last used timestamp

### ConnectionPool Class

#### Methods
- `ConnectionPool(const ConnectionPoolConfig& config)` - Constructor
- `~ConnectionPool()` - Destructor
- `bool initialize()` - Initialize the pool
- `void shutdown()` - Shutdown the pool
- `std::unique_ptr<MySQLConnection> getConnection(timeout)` - Get connection (synchronous)
- `void getConnectionAsync(callback, timeout)` - Get connection (asynchronous)
- `void returnConnection(connection)` - Return connection to pool
- `ConnectionPoolStats getStats() const` - Get statistics
- `size_t performHealthCheck()` - Force health check
- `bool addConnection()` - Add new connection
- `void removeExcessConnections()` - Remove excess connections
- `bool isHealthy() const` - Check pool health
- `void setCustomValidator(validator)` - Set custom validator
- `void setCustomFactory(factory)` - Set custom factory

### ConnectionPoolConfig Structure

#### Database Configuration
- `std::string host` - Database host
- `unsigned int port` - Database port
- `std::string database` - Database name
- `std::string username` - Database username
- `std::string password` - Database password

#### Pool Size Configuration
- `size_t initialPoolSize` - Initial connections (default: 5)
- `size_t maxPoolSize` - Maximum connections (default: 20)
- `size_t minPoolSize` - Minimum connections (default: 2)

#### Timeout Configuration (seconds)
- `unsigned int connectionTimeout` - Connection creation timeout (default: 30)
- `unsigned int idleTimeout` - Idle connection timeout (default: 300)
- `unsigned int maxLifetime` - Maximum connection lifetime (default: 3600)
- `unsigned int validationTimeout` - Validation timeout (default: 5)

#### Health Check Configuration
- `bool enableHealthCheck` - Enable health checks (default: true)
- `unsigned int healthCheckInterval` - Health check interval (default: 60)
- `unsigned int maxValidationRetries` - Validation retry attempts (default: 3)

#### Thread Configuration
- `size_t workerThreadCount` - Worker thread count (default: 2)

#### Logging Configuration
- `std::string logLevel` - Logging level (default: "INFO")
- `std::string logPattern` - Log pattern (default: timestamp with thread info)

#### Validation Configuration
- `std::string validationQuery` - Validation query (default: "SELECT 1")

#### Retry Configuration
- `unsigned int maxRetries` - Maximum connection retries (default: 3)
- `unsigned int retryDelay` - Retry delay in seconds (default: 1)

### ConnectionPoolStats Structure

#### Atomic Counters
- `std::atomic<size_t> totalConnections` - Total connections created
- `std::atomic<size_t> activeConnections` - Currently active connections
- `std::atomic<size_t> idleConnections` - Currently idle connections
- `std::atomic<size_t> failedConnections` - Total failed connection attempts
- `std::atomic<size_t> totalRequests` - Total connection requests
- `std::atomic<size_t> successfulRequests` - Successful requests
- `std::atomic<size_t> timedOutRequests` - Timed out requests
- `std::atomic<size_t> healthCheckFailures` - Health check failures

#### Time Information
- `std::chrono::steady_clock::time_point lastHealthCheck` - Last health check time

#### Calculated Metrics
- `double getUtilization() const` - Connection utilization percentage
- `double getSuccessRate() const` - Success rate percentage

## Thread Safety

The connection pool is designed to be thread-safe:

- All public methods are thread-safe
- Multiple threads can safely call `getConnection()` concurrently
- Multiple threads can safely call `returnConnection()` concurrently
- Statistics are updated atomically
- Connection validation is performed under mutex protection

## Error Handling

### Connection Failures
- Automatic retry with exponential backoff
- Connection validation before returning to pool
- Health check detection of broken connections
- Graceful degradation when pool is exhausted

### Resource Management
- Automatic cleanup of failed connections
- Proper thread termination during shutdown
- Memory leak prevention through RAII

### Logging
- Comprehensive error logging using log4cplus
- Configurable log levels
- Performance monitoring logs
- Debug information for troubleshooting

## Performance Considerations

### Optimization Tips
1. **Pool Sizing**: Configure appropriate min/max pool sizes based on expected load
2. **Timeouts**: Set reasonable timeouts to prevent thread starvation
3. **Health Checks**: Balance between detection speed and overhead
4. **Validation**: Use lightweight validation queries
5. **Async Operations**: Use async API for I/O-bound operations

### Monitoring
- Track connection utilization and success rates
- Monitor health check failure rates
- Watch for timeout patterns
- Analyze connection lifecycle metrics

## Troubleshooting

### Common Issues

#### Connection Timeouts
- Check database server availability
- Verify connection parameters
- Increase timeout values
- Check network connectivity

#### High Resource Usage
- Reduce max pool size
- Increase idle timeout
- Optimize health check interval
- Review connection lifetime settings

#### Health Check Failures
- Verify validation query
- Check database permissions
- Monitor database load
- Adjust validation timeout

### Debugging

Enable debug logging:
```cpp
config.logLevel = "DEBUG";
```

Force health check:
```cpp
size_t healthy = pool.performHealthCheck();
```

Monitor statistics:
```cpp
auto stats = pool.getStats();
// Check utilization, success rate, etc.
```

## Best Practices

1. **Initialization**: Always call `initialize()` before using the pool
2. **Shutdown**: Always call `shutdown()` to clean up resources
3. **Connection Usage**: Use connection objects within scope for automatic return
4. **Timeout Configuration**: Set appropriate timeouts for your use case
5. **Health Monitoring**: Enable health checks for production systems
6. **Error Handling**: Always check for null connection returns
7. **Resource Limits**: Set reasonable pool size limits
8. **Custom Validation**: Implement application-specific validation logic
9. **Statistics Monitoring**: Regularly monitor pool statistics
10. **Graceful Degradation**: Design for cases when connections are unavailable

## License

This implementation is part of the VS project and follows the project's licensing terms.

## Contributing

When contributing to this connection pool implementation:

1. Maintain thread-safety guarantees
2. Add comprehensive error handling
3. Include logging for significant operations
4. Add unit tests for new functionality
5. Update documentation for API changes
6. Consider performance implications
7. Follow existing code style and patterns
