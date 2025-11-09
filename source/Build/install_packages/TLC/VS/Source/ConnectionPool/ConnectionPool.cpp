#include "ConnectionPool.h"
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <algorithm>

namespace VS {
namespace Database {

ConnectionPool::ConnectionPool(const ConnectionPoolConfig& config)
    : config_(config)
    , logger_(log4cplus::Logger::getInstance("VS.Database.ConnectionPool"))
    , isRunning_(false)
    , stats_() {
    
    // Initialize logging
    initializeLogging();
    
    LOG4CPLUS_INFO(logger_, "ConnectionPool created with configuration:");
    LOG4CPLUS_INFO(logger_, "  - Host: " << config_.host);
    LOG4CPLUS_INFO(logger_, "  - Port: " << config_.port);
    LOG4CPLUS_INFO(logger_, "  - Database: " << config_.database);
    LOG4CPLUS_INFO(logger_, "  - Initial pool size: " << config_.initialPoolSize);
    LOG4CPLUS_INFO(logger_, "  - Max pool size: " << config_.maxPoolSize);
    LOG4CPLUS_INFO(logger_, "  - Min pool size: " << config_.minPoolSize);
}

ConnectionPool::~ConnectionPool() {
    shutdown();
}

bool ConnectionPool::initialize() {
    if (isRunning_) {
        LOG4CPLUS_WARN(logger_, "ConnectionPool already initialized");
        return true;
    }
    
    LOG4CPLUS_INFO(logger_, "Initializing ConnectionPool...");
    isRunning_ = true;
    
    // Create initial connections
    for (size_t i = 0; i < config_.initialPoolSize; ++i) {
        if (!addConnection()) {
            LOG4CPLUS_ERROR(logger_, "Failed to create initial connection " << i);
            isRunning_ = false;
            return false;
        }
    }
    
    LOG4CPLUS_INFO(logger_, "Created " << config_.initialPoolSize << " initial connections");
    
    // Start background worker threads
    if (config_.enableHealthCheck) {
        workerThreads_.emplace_back(&ConnectionPool::healthCheckWorker, this);
        LOG4CPLUS_INFO(logger_, "Started health check worker thread");
    }
    
    workerThreads_.emplace_back(&ConnectionPool::connectionMonitorWorker, this);
    LOG4CPLUS_INFO(logger_, "Started connection monitor worker thread");
    
    // Start async request workers
    for (size_t i = 0; i < config_.workerThreadCount; ++i) {
        workerThreads_.emplace_back(&ConnectionPool::asyncRequestWorker, this);
    }
    LOG4CPLUS_INFO(logger_, "Started " << config_.workerThreadCount << " async request worker threads");
    
    // Log initial statistics
    logStats();
    
    LOG4CPLUS_INFO(logger_, "ConnectionPool initialized successfully");
    return true;
}

void ConnectionPool::shutdown() {
    if (!isRunning_) {
        return;
    }
    
    LOG4CPLUS_INFO(logger_, "Shutting down ConnectionPool...");
    isRunning_ = false;
    
    // Wake up all waiting threads
    poolCondition_.notify_all();
    
    // Wait for worker threads to finish
    for (auto& thread : workerThreads_) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    workerThreads_.clear();
    
    // Close all connections
    std::unique_lock<std::mutex> lock(poolMutex_);
    while (!availableConnections_.empty()) {
        auto connection = std::move(availableConnections_.front());
        availableConnections_.pop();
        if (connection) {
            closeConnection(std::move(connection));
        }
    }
    
    allConnections_.clear();
    lock.unlock();
    
    // Log final statistics
    logStats();
    
    LOG4CPLUS_INFO(logger_, "ConnectionPool shutdown complete");
}

std::unique_ptr<MySQLConnection> ConnectionPool::getConnection(
    std::chrono::seconds timeout) {
    
    stats_.totalRequests++;
    auto startTime = std::chrono::steady_clock::now();
    auto timeoutTime = startTime + timeout;
    
    std::unique_lock<std::mutex> lock(poolMutex_);
    
    while (true) {
        // Check if we have available connections
        if (!availableConnections_.empty()) {
            auto connection = std::move(availableConnections_.front());
            availableConnections_.pop();
            
            // Validate the connection
            if (validateConnection(connection->getConnection())) {
                stats_.activeConnections++;
                stats_.idleConnections--;
                stats_.successfulRequests++;
                connection->updateLastUsed();
                
                LOG4CPLUS_DEBUG(logger_, "Acquired connection ID: " << connection->getId());
                return connection;
            } else {
                // Connection is invalid, close it and continue
                LOG4CPLUS_WARN(logger_, "Connection ID " << connection->getId() << " failed validation, closing");
                closeConnection(std::move(connection));
            }
        }
        
        // Check if we can create more connections
        size_t totalConnections = allConnections_.size();
        if (totalConnections < config_.maxPoolSize) {
            // Try to create a new connection
            lock.unlock();
            if (addConnection()) {
                lock.lock();
                continue; // Go back to try to acquire the new connection
            } else {
                lock.lock();
            }
        }
        
        // Check for timeout
        auto now = std::chrono::steady_clock::now();
        if (now >= timeoutTime) {
            stats_.timedOutRequests++;
            LOG4CPLUS_ERROR(logger_, "Connection request timed out after " << timeout.count() << " seconds");
            return nullptr;
        }
        
        // Wait for a connection to become available or for a timeout
        auto remainingTime = std::chrono::duration_cast<std::chrono::milliseconds>(timeoutTime - now);
        poolCondition_.wait_for(lock, remainingTime);
    }
}

void ConnectionPool::getConnectionAsync(
    std::function<void(std::unique_ptr<MySQLConnection>)> callback,
    std::chrono::seconds timeout) {
    
    // For now, just run the synchronous version in a separate thread
    // In a more sophisticated implementation, you would use a proper async task queue
    std::thread([this, callback, timeout]() {
        auto connection = getConnection(timeout);
        callback(std::move(connection));
    }).detach();
}

void ConnectionPool::returnConnection(std::unique_ptr<MySQLConnection> connection) {
    if (!connection) {
        return;
    }
    
    std::unique_lock<std::mutex> lock(poolMutex_);
    
    // Update statistics
    stats_.activeConnections--;
    stats_.idleConnections++;
    
    // Check if the connection is still valid
    if (validateConnection(connection->getConnection())) {
        // Add back to available pool
        availableConnections_.push(std::move(connection));
        poolCondition_.notify_one();
        
        LOG4CPLUS_DEBUG(logger_, "Returned connection to pool");
    } else {
        // Connection is invalid, close it
        LOG4CPLUS_WARN(logger_, "Returned connection ID " << connection->getId() << " is invalid, closing");
        closeConnection(std::move(connection));
    }
}

ConnectionPoolStatsSnapshot ConnectionPool::getStats() const {
    ConnectionPoolStatsSnapshot snapshot;
    snapshot.totalConnections = stats_.totalConnections.load();
    snapshot.activeConnections = stats_.activeConnections.load();
    snapshot.idleConnections = stats_.idleConnections.load();
    snapshot.failedConnections = stats_.failedConnections.load();
    snapshot.totalRequests = stats_.totalRequests.load();
    snapshot.successfulRequests = stats_.successfulRequests.load();
    snapshot.timedOutRequests = stats_.timedOutRequests.load();
    snapshot.healthCheckFailures = stats_.healthCheckFailures.load();
    snapshot.lastHealthCheck = stats_.lastHealthCheck;
    return snapshot;
}

size_t ConnectionPool::performHealthCheck() {
    LOG4CPLUS_INFO(logger_, "Performing health check on all connections...");
    size_t healthyCount = 0;
    size_t totalConnections = 0;
    
    std::unique_lock<std::mutex> lock(poolMutex_);
    
    for (auto& connection : allConnections_) {
        totalConnections++;
        if (validateConnection(connection->getConnection())) {
            connection->resetPotentiallyInvalid();
            healthyCount++;
        } else {
            connection->markAsPotentiallyInvalid();
            stats_.healthCheckFailures++;
        }
    }
    
    lock.unlock();
    
    stats_.lastHealthCheck = std::chrono::steady_clock::now();
    
    LOG4CPLUS_INFO(logger_, "Health check complete: " << healthyCount << "/" << totalConnections 
                    << " connections are healthy");
    
    // Log statistics
    logStats();
    
    return healthyCount;
}

bool ConnectionPool::addConnection() {
    try {
        sql::Connection* conn = createNewConnection();
        if (!conn) {
            stats_.failedConnections++;
            return false;
        }
        
        int connectionId = static_cast<int>(allConnections_.size()) + 1;
        auto createdAt = std::chrono::steady_clock::now();
        
        auto mysqlConnection = std::unique_ptr<MySQLConnection>(new MySQLConnection(conn, connectionId, createdAt));
        
        // Validate the new connection
        if (!validateConnection(conn)) {
            LOG4CPLUS_ERROR(logger_, "New connection failed validation");
            delete conn;
            stats_.failedConnections++;
            return false;
        }
        
        // Add to pool
        std::unique_lock<std::mutex> lock(poolMutex_);
        availableConnections_.push(std::move(mysqlConnection));
        allConnections_.push_back(std::move(availableConnections_.back()));
        availableConnections_.pop();
        
        stats_.totalConnections++;
        stats_.idleConnections++;
        
        poolCondition_.notify_one();
        
        LOG4CPLUS_DEBUG(logger_, "Added new connection ID: " << connectionId);
        return true;
        
    } catch (const std::exception& e) {
        LOG4CPLUS_ERROR(logger_, "Failed to create connection: " << e.what());
        stats_.failedConnections++;
        return false;
    }
}

void ConnectionPool::removeExcessConnections() {
    std::unique_lock<std::mutex> lock(poolMutex_);
    
    if (availableConnections_.size() <= config_.minPoolSize) {
        return; // Nothing to remove
    }
    
    // Remove connections that have been idle too long
    std::queue<std::unique_ptr<MySQLConnection>> newAvailable;
    
    while (!availableConnections_.empty()) {
        auto connection = std::move(availableConnections_.front());
        availableConnections_.pop();
        
        long long idleTime = connection->getSecondsSinceLastUsed();
        if (idleTime > static_cast<long long>(config_.idleTimeout) && 
            availableConnections_.size() + newAvailable.size() >= config_.minPoolSize) {
            // Remove this connection
            LOG4CPLUS_DEBUG(logger_, "Removing excess idle connection ID: " << connection->getId());
            closeConnection(std::move(connection));
        } else {
            newAvailable.push(std::move(connection));
        }
    }
    
    availableConnections_ = std::move(newAvailable);
}

bool ConnectionPool::isHealthy() const {
    return isRunning_ && 
           (stats_.activeConnections.load() + stats_.idleConnections.load()) > 0 &&
           (stats_.successfulRequests.load() / std::max(stats_.totalRequests.load(), size_t(1))) > 0.8; // At least 80% success rate
}

void ConnectionPool::healthCheckWorker() {
    LOG4CPLUS_INFO(logger_, "Health check worker thread started");
    
    while (isRunning_) {
        try {
            std::this_thread::sleep_for(std::chrono::seconds(config_.healthCheckInterval));
            
            if (isRunning_) {
                performHealthCheck();
            }
        } catch (const std::exception& e) {
            LOG4CPLUS_ERROR(logger_, "Health check worker error: " << e.what());
        }
    }
    
    LOG4CPLUS_INFO(logger_, "Health check worker thread stopped");
}

void ConnectionPool::connectionMonitorWorker() {
    LOG4CPLUS_INFO(logger_, "Connection monitor worker thread started");
    
    while (isRunning_) {
        try {
            std::this_thread::sleep_for(std::chrono::seconds(30)); // Check every 30 seconds
            
            if (isRunning_) {
                removeExcessConnections();
                cleanupIdleConnections();
            }
        } catch (const std::exception& e) {
            LOG4CPLUS_ERROR(logger_, "Connection monitor worker error: " << e.what());
        }
    }
    
    LOG4CPLUS_INFO(logger_, "Connection monitor worker thread stopped");
}

void ConnectionPool::asyncRequestWorker() {
    LOG4CPLUS_DEBUG(logger_, "Async request worker thread started");
    
    // For now, this is a placeholder for a more sophisticated async implementation
    // In a real implementation, you would have a task queue here
    
    while (isRunning_) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    LOG4CPLUS_DEBUG(logger_, "Async request worker thread stopped");
}

sql::Connection* ConnectionPool::createNewConnection() {
    std::string connStr = buildConnectionString();
    constexpr unsigned int MAX_EXPONENTIAL_BACKOFF_ATTEMPTS = 3; // Limit to first 3 attempts for exponential backoff
    
    for (unsigned int attempt = 0; attempt <= config_.maxRetries; ++attempt) {
        try {
            // Add a small delay before each attempt to reduce memory pressure
            if (attempt > 0) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            
            sql::Driver* driver = get_driver_instance();
            sql::Connection* conn = driver->connect(connStr, config_.username, config_.password);
            
            if (conn) {
                // Set connection options
                conn->setAutoCommit(true);
                return conn;
            }
        } catch (const std::bad_alloc& e) {
            LOG4CPLUS_ERROR(logger_, "Connection attempt " << (attempt + 1) 
                            << " failed due to memory allocation error: " << e.what());
            stats_.failedConnections++;
        } catch (const sql::SQLException& e) {
            LOG4CPLUS_ERROR(logger_, "Connection attempt " << (attempt + 1) 
                            << " failed: " << e.what());
        } catch (const std::exception& e) {
            LOG4CPLUS_ERROR(logger_, "Connection attempt " << (attempt + 1) 
                            << " failed with unexpected error: " << e.what());
        }
        
        // Don't retry if we've reached max retries or if this was a memory allocation error
        if (attempt >= config_.maxRetries) {
            break;
        }
        
        // Use exponential backoff for first few attempts, then constant delay
        if (attempt < MAX_EXPONENTIAL_BACKOFF_ATTEMPTS) {
            // Exponential backoff: 1s, 2s, 4s, etc.
            auto backoffDelay = std::chrono::seconds(1 << attempt);
            LOG4CPLUS_INFO(logger_, "Retrying connection in " << backoffDelay.count() << " seconds (attempt " 
                           << (attempt + 2) << "/" << (config_.maxRetries + 1) << ")");
            std::this_thread::sleep_for(backoffDelay);
        } else {
            // Constant delay for remaining attempts
            LOG4CPLUS_INFO(logger_, "Retrying connection in " << config_.retryDelay 
                           << " seconds (attempt " << (attempt + 2) << "/" << (config_.maxRetries + 1) << ")");
            std::this_thread::sleep_for(std::chrono::seconds(config_.retryDelay));
        }
    }
    
    LOG4CPLUS_ERROR(logger_, "All connection attempts failed after " << (config_.maxRetries + 1) << " tries");
    return nullptr;
}

bool ConnectionPool::validateConnection(sql::Connection* connection) {
    if (!connection) {
        return false;
    }
    
    try {
        // Use custom validator if provided
        if (customValidator_) {
            return customValidator_(connection);
        }
        
        // Use default validation query
        std::unique_ptr<sql::Statement> stmt(connection->createStatement());
        std::unique_ptr<sql::ResultSet> result(stmt->executeQuery(config_.validationQuery));
        return result != nullptr;
        
    } catch (const sql::SQLException& e) {
        LOG4CPLUS_ERROR(logger_, "Connection validation failed: " << e.what());
        return false;
    }
}

void ConnectionPool::closeConnection(std::unique_ptr<MySQLConnection> connection) {
    if (!connection) {
        return;
    }
    
    try {
        stats_.totalConnections--;
        stats_.idleConnections--;
        
        LOG4CPLUS_DEBUG(logger_, "Closing connection ID: " << connection->getId());
        connection.reset(); // This will call the destructor which closes the connection
    } catch (const std::exception& e) {
        LOG4CPLUS_ERROR(logger_, "Error closing connection: " << e.what());
    }
}

void ConnectionPool::cleanupIdleConnections() {
    std::unique_lock<std::mutex> lock(poolMutex_);
    
    // Check connections for timeout and excessive lifetime
    auto now = std::chrono::steady_clock::now();
    
    // Remove connections from available pool
    std::queue<std::unique_ptr<MySQLConnection>> newAvailable;
    
    while (!availableConnections_.empty()) {
        auto connection = std::move(availableConnections_.front());
        availableConnections_.pop();
        
        long long idleTime = connection->getSecondsSinceLastUsed();
        long long lifeTime = connection->getSecondsSinceCreation();
        
        bool shouldClose = false;
        std::string reason;
        
        if (lifeTime > static_cast<long long>(config_.maxLifetime)) {
            shouldClose = true;
            reason = "exceeded max lifetime";
        } else if (idleTime > static_cast<long long>(config_.idleTimeout)) {
            shouldClose = true;
            reason = "exceeded idle timeout";
        }
        
        if (shouldClose) {
            LOG4CPLUS_DEBUG(logger_, "Closing connection ID: " << connection->getId() 
                            << " (" << reason << ")");
            closeConnection(std::move(connection));
        } else {
            newAvailable.push(std::move(connection));
        }
    }
    
    availableConnections_ = std::move(newAvailable);
}

void ConnectionPool::logStats() const {
    LOG4CPLUS_INFO(logger_, "=== Connection Pool Statistics ===");
    LOG4CPLUS_INFO(logger_, "Total connections created: " << stats_.totalConnections.load());
    LOG4CPLUS_INFO(logger_, "Active connections: " << stats_.activeConnections.load());
    LOG4CPLUS_INFO(logger_, "Idle connections: " << stats_.idleConnections.load());
    LOG4CPLUS_INFO(logger_, "Failed connections: " << stats_.failedConnections.load());
    LOG4CPLUS_INFO(logger_, "Total requests: " << stats_.totalRequests.load());
    LOG4CPLUS_INFO(logger_, "Successful requests: " << stats_.successfulRequests.load());
    LOG4CPLUS_INFO(logger_, "Timed out requests: " << stats_.timedOutRequests.load());
    
    size_t total = stats_.activeConnections.load() + stats_.idleConnections.load();
    size_t successful = stats_.successfulRequests.load();
    size_t totalReqs = stats_.totalRequests.load();
    double utilization = total > 0 ? static_cast<double>(stats_.activeConnections.load()) / total : 0.0;
    double successRate = totalReqs > 0 ? static_cast<double>(successful) / totalReqs : 0.0;
    
    LOG4CPLUS_INFO(logger_, "Utilization: " << (utilization * 100) << "%");
    LOG4CPLUS_INFO(logger_, "Success rate: " << (successRate * 100) << "%");
    LOG4CPLUS_INFO(logger_, "Health check failures: " << stats_.healthCheckFailures.load());
    LOG4CPLUS_INFO(logger_, "=================================");
}

void ConnectionPool::initializeLogging() {
    // Set up log4cplus if not already configured
    // This is a basic setup - in production you might want more sophisticated logging
    if (!log4cplus::Logger::getInstance("VS.Database.ConnectionPool").getAppender("")) {
        log4cplus::SharedAppenderPtr appender(new log4cplus::ConsoleAppender());
        appender->setLayout(std::unique_ptr<log4cplus::Layout>(
            new log4cplus::PatternLayout(config_.logPattern)));
        
        logger_.addAppender(appender);
        logger_.setLogLevel(log4cplus::INFO_LOG_LEVEL);
    }
}

std::string ConnectionPool::buildConnectionString() const {
    std::ostringstream oss;
    oss << "tcp://" << config_.host << ":" << config_.port << "/" << config_.database;
    return oss.str();
}

} // namespace Database
} // namespace VS
