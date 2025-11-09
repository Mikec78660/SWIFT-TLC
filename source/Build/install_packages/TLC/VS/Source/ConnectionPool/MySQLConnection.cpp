#include "MySQLConnection.h"
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <vector>

namespace VS {
namespace Database {

MySQLConnection::MySQLConnection(sql::Connection* connection, int id, std::chrono::steady_clock::time_point createdAt)
    : connection_(connection)
    , id_(id)
    , createdAt_(createdAt)
    , lastUsed_(std::chrono::steady_clock::now())
    , isPotentiallyInvalid_(false) {
}

MySQLConnection::~MySQLConnection() {
    if (connection_ != nullptr) {
        connection_->close();
        delete connection_;
        connection_ = nullptr;
    }
}

MySQLConnection::MySQLConnection(MySQLConnection&& other) noexcept
    : connection_(other.connection_)
    , id_(other.id_)
    , createdAt_(other.createdAt_)
    , lastUsed_(other.lastUsed_)
    , isPotentiallyInvalid_(other.isPotentiallyInvalid_.load()) {
    other.connection_ = nullptr;
}

MySQLConnection& MySQLConnection::operator=(MySQLConnection&& other) noexcept {
    if (this != &other) {
        // Clean up current connection
        if (connection_ != nullptr) {
            connection_->close();
            delete connection_;
        }
        
        // Move data from other
        connection_ = other.connection_;
        id_ = other.id_;
        createdAt_ = other.createdAt_;
        lastUsed_ = other.lastUsed_;
        isPotentiallyInvalid_.store(other.isPotentiallyInvalid_.load());
        
        // Reset other
        other.connection_ = nullptr;
    }
    return *this;
}

bool MySQLConnection::executeQuery(const std::string& query) {
    if (!connection_ || !isHealthy()) {
        return false;
    }
    
    updateLastUsed();
    
    try {
        std::unique_ptr<sql::Statement> stmt(connection_->createStatement());
        stmt->execute(query);
        return true;
    } catch (const sql::SQLException& e) {
        std::cerr << "MySQL query failed: " << e.what() << std::endl;
        return false;
    }
}

std::unique_ptr<sql::ResultSet> MySQLConnection::executeQueryWithResult(const std::string& query) {
    if (!connection_ || !isHealthy()) {
        return nullptr;
    }
    
    updateLastUsed();
    
    try {
        std::unique_ptr<sql::Statement> stmt(connection_->createStatement());
        std::unique_ptr<sql::ResultSet> result(stmt->executeQuery(query));
        return result;
    } catch (const sql::SQLException& e) {
        std::cerr << "MySQL query failed: " << e.what() << std::endl;
        return nullptr;
    }
}

bool MySQLConnection::executePreparedQuery(const std::string& query, const std::vector<std::string>& params) {
    if (!connection_ || !isHealthy()) {
        return false;
    }
    
    updateLastUsed();
    
    try {
        std::unique_ptr<sql::PreparedStatement> stmt(connection_->prepareStatement(query));
        
        // Set parameters
        for (size_t i = 0; i < params.size(); ++i) {
            stmt->setString(static_cast<unsigned int>(i + 1), params[i]);
        }
        
        stmt->execute();
        return true;
    } catch (const sql::SQLException& e) {
        std::cerr << "MySQL prepared query failed: " << e.what() << std::endl;
        return false;
    }
}

bool MySQLConnection::beginTransaction() {
    if (!connection_ || !isHealthy()) {
        return false;
    }
    
    try {
        connection_->setAutoCommit(false);
        return true;
    } catch (const sql::SQLException& e) {
        std::cerr << "MySQL begin transaction failed: " << e.what() << std::endl;
        return false;
    }
}

bool MySQLConnection::commit() {
    if (!connection_ || !isHealthy()) {
        return false;
    }
    
    try {
        connection_->commit();
        connection_->setAutoCommit(true);
        return true;
    } catch (const sql::SQLException& e) {
        std::cerr << "MySQL commit failed: " << e.what() << std::endl;
        return false;
    }
}

bool MySQLConnection::rollback() {
    if (!connection_ || !isHealthy()) {
        return false;
    }
    
    try {
        connection_->rollback();
        connection_->setAutoCommit(true);
        return true;
    } catch (const sql::SQLException& e) {
        std::cerr << "MySQL rollback failed: " << e.what() << std::endl;
        return false;
    }
}

bool MySQLConnection::isHealthy() const {
    if (!connection_) {
        return false;
    }
    
    // Check if connection is marked as potentially invalid
    if (isPotentiallyInvalid_) {
        return false;
    }
    
    // Simple health check - try a lightweight query
    try {
        std::unique_ptr<sql::Statement> stmt(connection_->createStatement());
        std::unique_ptr<sql::ResultSet> result(stmt->executeQuery("SELECT 1"));
        return result != nullptr;
    } catch (const sql::SQLException& e) {
        std::cerr << "MySQL health check failed: " << e.what() << std::endl;
        return false;
    }
}

void MySQLConnection::updateLastUsed() {
    lastUsed_ = std::chrono::steady_clock::now();
}

long long MySQLConnection::getSecondsSinceLastUsed() const {
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::seconds>(now - lastUsed_).count();
}

long long MySQLConnection::getSecondsSinceCreation() const {
    auto now = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::seconds>(now - createdAt_).count();
}

} // namespace Database
} // namespace VS
