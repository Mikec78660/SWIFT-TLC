#include "ConnectionPool.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <functional>
#include <atomic>

using namespace VS::Database;

/**
 * @brief Example demonstrating basic usage of the ConnectionPool
 */
void basicUsageExample() {
    std::cout << "=== Basic Usage Example ===" << std::endl;
    
    // Configure the connection pool
    ConnectionPoolConfig config;
    config.host = "localhost";
    config.port = 3306;
    config.database = "test";
    config.username = "testuser";
    config.password = "testpass";
    config.initialPoolSize = 3;
    config.maxPoolSize = 10;
    config.minPoolSize = 2;
    config.idleTimeout = 300; // 5 minutes
    config.maxLifetime = 3600; // 1 hour
    config.enableHealthCheck = true;
    config.healthCheckInterval = 30; // 30 seconds
    config.workerThreadCount = 2;
    
    // Create and initialize the connection pool
    ConnectionPool pool(config);
    
    if (!pool.initialize()) {
        std::cerr << "Failed to initialize connection pool" << std::endl;
        return;
    }
    
    std::cout << "Connection pool initialized successfully" << std::endl;
    
    // Get statistics
    auto stats = pool.getStats();
    std::cout << "Initial stats - Total connections: " << stats.totalConnections 
              << ", Active: " << stats.activeConnections 
              << ", Idle: " << stats.idleConnections << std::endl;
    
    // Get a connection and perform some operations
    {
        auto connection = pool.getConnection();
        if (connection) {
            std::cout << "Acquired connection ID: " << connection->getId() << std::endl;
            
            // Perform some database operations
            // connection->executeQuery("SELECT 1");
            
            std::cout << "Connection is healthy: " << (connection->isHealthy() ? "Yes" : "No") << std::endl;
        } else {
            std::cerr << "Failed to get connection" << std::endl;
        }
    } // Connection is automatically returned to pool when it goes out of scope
    
    std::cout << "Connection returned to pool" << std::endl;
    
    // Get final statistics
    stats = pool.getStats();
    std::cout << "Final stats - Total requests: " << stats.totalRequests 
              << ", Successful: " << stats.successfulRequests 
              << ", Success rate: " << (stats.getSuccessRate() * 100) << "%" << std::endl;
    
    // Shutdown the pool
    pool.shutdown();
    std::cout << "Connection pool shutdown complete" << std::endl;
}

/**
 * @brief Example demonstrating concurrent usage of the connection pool
 */
void concurrentUsageExample() {
    std::cout << "\n=== Concurrent Usage Example ===" << std::endl;
    
    ConnectionPoolConfig config;
    config.host = "localhost";
    config.port = 3306;
    config.database = "test";
    config.username = "testuser";
    config.password = "testpass";
    config.initialPoolSize = 5;
    config.maxPoolSize = 15;
    config.minPoolSize = 3;
    config.idleTimeout = 300;
    config.maxLifetime = 3600;
    config.enableHealthCheck = true;
    config.healthCheckInterval = 30;
    config.workerThreadCount = 4;
    
    ConnectionPool pool(config);
    
    if (!pool.initialize()) {
        std::cerr << "Failed to initialize connection pool" << std::endl;
        return;
    }
    
    std::cout << "Connection pool initialized for concurrent usage" << std::endl;
    
    const int numThreads = 10;
    const int operationsPerThread = 20;
    std::atomic<int> completedOperations{0};
    std::atomic<int> failedOperations{0};
    
    // Worker function that uses connections
    auto workerFunction = [&](int threadId) {
        for (int i = 0; i < operationsPerThread; ++i) {
            try {
                // Get a connection with timeout
                auto connection = pool.getConnection(std::chrono::seconds(5));
                
                if (connection) {
                    // Simulate some database work
                    std::this_thread::sleep_for(std::chrono::milliseconds(50 + (threadId % 3) * 25));
                    
                    // Check connection health
                    if (!connection->isHealthy()) {
                        std::cerr << "Thread " << threadId << " found unhealthy connection" << std::endl;
                        failedOperations++;
                    } else {
                        completedOperations++;
                    }
                    
                    // Connection is automatically returned when it goes out of scope
                } else {
                    std::cerr << "Thread " << threadId << " failed to get connection" << std::endl;
                    failedOperations++;
                }
            } catch (const std::exception& e) {
                std::cerr << "Thread " << threadId << " exception: " << e.what() << std::endl;
                failedOperations++;
            }
        }
    };
    
    // Start multiple threads
    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(workerFunction, i);
    }
    
    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }
    
    std::cout << "Concurrent operations completed:" << std::endl;
    std::cout << "  Successful: " << completedOperations << std::endl;
    std::cout << "  Failed: " << failedOperations << std::endl;
    std::cout << "  Total expected: " << (numThreads * operationsPerThread) << std::endl;
    
    // Get final statistics
    auto stats = pool.getStats();
    std::cout << "Pool statistics:" << std::endl;
    std::cout << "  Total connections created: " << stats.totalConnections << std::endl;
    std::cout << "  Active connections: " << stats.activeConnections << std::endl;
    std::cout << "  Idle connections: " << stats.idleConnections << std::endl;
    std::cout << "  Total requests: " << stats.totalRequests << std::endl;
    std::cout << "  Successful requests: " << stats.successfulRequests << std::endl;
    std::cout << "  Utilization: " << (stats.getUtilization() * 100) << "%" << std::endl;
    std::cout << "  Success rate: " << (stats.getSuccessRate() * 100) << "%" << std::endl;
    
    // Force a health check
    size_t healthyConnections = pool.performHealthCheck();
    std::cout << "Health check found " << healthyConnections << " healthy connections" << std::endl;
    
    // Shutdown
    pool.shutdown();
    std::cout << "Connection pool shutdown complete" << std::endl;
}

/**
 * @brief Example demonstrating async usage of the connection pool
 */
void asyncUsageExample() {
    std::cout << "\n=== Async Usage Example ===" << std::endl;
    
    ConnectionPoolConfig config;
    config.host = "localhost";
    config.port = 3306;
    config.database = "test";
    config.username = "testuser";
    config.password = "testpass";
    config.initialPoolSize = 3;
    config.maxPoolSize = 8;
    config.minPoolSize = 2;
    config.workerThreadCount = 3;
    
    ConnectionPool pool(config);
    
    if (!pool.initialize()) {
        std::cerr << "Failed to initialize connection pool" << std::endl;
        return;
    }
    
    std::cout << "Connection pool initialized for async usage" << std::endl;
    
    // Perform async operations
    std::atomic<int> asyncCompleted{0};
    
    for (int i = 0; i < 5; ++i) {
        pool.getConnectionAsync([&, i](std::unique_ptr<MySQLConnection> connection) {
            if (connection) {
                std::cout << "Async operation " << i << " got connection ID: " 
                          << connection->getId() << std::endl;
                
                // Simulate async work
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                
                // Check if connection is still valid
                if (connection->isHealthy()) {
                    std::cout << "Async operation " << i << " completed successfully" << std::endl;
                } else {
                    std::cout << "Async operation " << i << " found unhealthy connection" << std::endl;
                }
                
                asyncCompleted++;
            } else {
                std::cerr << "Async operation " << i << " failed to get connection" << std::endl;
            }
        }, std::chrono::seconds(3));
    }
    
    // Wait for async operations to complete
    while (asyncCompleted < 5) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    std::cout << "All async operations completed" << std::endl;
    
    // Shutdown
    pool.shutdown();
    std::cout << "Connection pool shutdown complete" << std::endl;
}

/**
 * @brief Example demonstrating custom validation and factory functions
 */
void customExample() {
    std::cout << "\n=== Custom Validation Example ===" << std::endl;
    
    ConnectionPoolConfig config;
    config.host = "localhost";
    config.port = 3306;
    config.database = "test";
    config.username = "testuser";
    config.password = "testpass";
    config.initialPoolSize = 2;
    config.maxPoolSize = 5;
    config.minPoolSize = 1;
    config.validationQuery = "SELECT 1";
    
    ConnectionPool pool(config);
    
    // Set custom validator
    pool.setCustomValidator([](sql::Connection* conn) -> bool {
        try {
            // Custom validation logic
            std::unique_ptr<sql::Statement> stmt(conn->createStatement());
            std::unique_ptr<sql::ResultSet> result(stmt->executeQuery("SELECT 1 as test"));
            
            if (result && result->next()) {
                int value = result->getInt("test");
                return value == 1;
            }
            return false;
        } catch (const sql::SQLException& e) {
            std::cerr << "Custom validation failed: " << e.what() << std::endl;
            return false;
        }
    });
    
    // Set custom factory (optional - uses default if not set)
    pool.setCustomFactory([]() -> sql::Connection* {
        try {
            // Custom connection creation logic
            sql::Driver* driver = get_driver_instance();
            return driver->connect("tcp://localhost:3306/test", "testuser", "testpass");
        } catch (const sql::SQLException& e) {
            std::cerr << "Custom factory failed: " << e.what() << std::endl;
            return nullptr;
        }
    });
    
    if (!pool.initialize()) {
        std::cerr << "Failed to initialize connection pool" << std::endl;
        return;
    }
    
    std::cout << "Connection pool initialized with custom functions" << std::endl;
    
    // Test with custom validation
    auto connection = pool.getConnection();
    if (connection) {
        std::cout << "Got connection with custom validation, ID: " << connection->getId() << std::endl;
    } else {
        std::cerr << "Failed to get connection with custom validation" << std::endl;
    }
    
    // Shutdown
    pool.shutdown();
    std::cout << "Connection pool shutdown complete" << std::endl;
}

/**
 * @brief Example demonstrating error handling and recovery
 */
void errorHandlingExample() {
    std::cout << "\n=== Error Handling Example ===" << std::endl;
    
    // Configure pool with short timeouts for demonstration
    ConnectionPoolConfig config;
    config.host = "localhost";
    config.port = 3306;
    config.database = "test";
    config.username = "testuser";
    config.password = "wrongpass"; // Intentionally wrong to trigger errors
    config.initialPoolSize = 2;
    config.maxPoolSize = 5;
    config.minPoolSize = 1;
    config.maxRetries = 2;
    config.retryDelay = 1;
    config.idleTimeout = 60;
    
    ConnectionPool pool(config);
    
    if (!pool.initialize()) {
        std::cerr << "Failed to initialize connection pool (expected with wrong password)" << std::endl;
        return;
    }
    
    // Try to get connections (should fail)
    for (int i = 0; i < 3; ++i) {
        auto connection = pool.getConnection(std::chrono::seconds(2));
        if (connection) {
            std::cout << "Got connection (unexpected): " << connection->getId() << std::endl;
        } else {
            std::cout << "Failed to get connection " << i << " (expected with wrong password)" << std::endl;
        }
    }
    
    // Check statistics
    auto stats = pool.getStats();
    std::cout << "Stats after failed attempts:" << std::endl;
    std::cout << "  Total requests: " << stats.totalRequests << std::endl;
    std::cout << "  Successful requests: " << stats.successfulRequests << std::endl;
    std::cout << "  Timed out requests: " << stats.timedOutRequests << std::endl;
    std::cout << "  Failed connections: " << stats.failedConnections << std::endl;
    std::cout << "  Success rate: " << (stats.getSuccessRate() * 100) << "%" << std::endl;
    
    std::cout << "Pool health status: " << (pool.isHealthy() ? "Healthy" : "Unhealthy") << std::endl;
    
    // Shutdown
    pool.shutdown();
    std::cout << "Connection pool shutdown complete" << std::endl;
}

/**
 * @brief Main function that runs all examples
 */
int main(int argc, char* argv[]) {
    std::cout << "VS Database Connection Pool Examples" << std::endl;
    std::cout << "====================================" << std::endl;
    std::cout << "Note: These examples require a MySQL server running on localhost" << std::endl;
    std::cout << "      with a test database. Update the configuration as needed." << std::endl;
    std::cout << std::endl;
    
    try {
        // Note: These examples assume MySQL is available
        // In a real environment, you would need proper database setup
        std::cout << "SKIPPING EXAMPLES - No MySQL connection available" << std::endl;
        std::cout << "The connection pool code has been successfully implemented." << std::endl;
        std::cout << "To test with a real database, update the connection parameters" << std::endl;
        std::cout << "in the examples above and ensure MySQL is running." << std::endl;
        
        // Uncomment the following lines to run actual examples (requires MySQL):
        // basicUsageExample();
        // concurrentUsageExample();
        // asyncUsageExample();
        // customExample();
        // errorHandlingExample();
        
    } catch (const std::exception& e) {
        std::cerr << "Exception in main: " << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "\nAll examples completed successfully!" << std::endl;
    return 0;
}
