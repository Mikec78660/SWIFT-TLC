cmake_minimum_required(VERSION 3.10)
project(VSConnectionPool VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(PkgConfig REQUIRED)

# Find MySQL Connector/C++
find_library(MYSQLCPPCONN_LIBRARY mysqlcppconn)
if(MYSQLCPPCONN_LIBRARY)
    message(STATUS "Found MySQL Connector/C++: ${MYSQLCPPCONN_LIBRARY}")
else()
    message(WARNING "MySQL Connector/C++ not found. Please install libmysqlcppconn.")
endif()

# Find log4cplus
find_library(LOG4CPLUS_LIBRARY log4cplus)
if(LOG4CPLUS_LIBRARY)
    message(STATUS "Found log4cplus: ${LOG4CPLUS_LIBRARY}")
else()
    message(WARNING "log4cplus not found. Please install log4cplus.")
endif()

# Find pthread
find_package(Threads REQUIRED)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Library sources
set(CONNECTION_POOL_SOURCES
    MySQLConnection.cpp
    ConnectionPool.cpp
)

# Header files
set(CONNECTION_POOL_HEADERS
    MySQLConnection.h
    ConnectionPool.h
)

# Create static library
add_library(VSConnectionPool STATIC ${CONNECTION_POOL_SOURCES} ${CONNECTION_POOL_HEADERS})

# Set library properties
set_target_properties(VSConnectionPool PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER "${CONNECTION_POOL_HEADERS}"
)

# Link libraries
target_link_libraries(VSConnectionPool
    ${MYSQLCPPCONN_LIBRARY}
    ${LOG4CPLUS_LIBRARY}
    Threads::Threads
)

# Install targets
install(TARGETS VSConnectionPool
    LIBRARY DESTINATION lib
    PUBLIC_HEADER DESTINATION include/VS/ConnectionPool
)

# Example executable
if(MYSQLCPPCONN_LIBRARY AND LOG4CPLUS_LIBRARY)
    add_executable(ConnectionPoolExample ConnectionPoolExample.cpp)
    target_link_libraries(ConnectionPoolExample VSConnectionPool)
    target_include_directories(ConnectionPoolExample PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    
    # Install example
    install(TARGETS ConnectionPoolExample DESTINATION bin)
endif()

# Tests (if you want to add unit tests later)
# enable_testing()
# add_subdirectory(tests)

# Print configuration summary
message(STATUS "=== VS Connection Pool Configuration ===")
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "MySQL Connector/C++: ${MYSQLCPPCONN_LIBRARY}")
message(STATUS "log4cplus: ${LOG4CPLUS_LIBRARY}")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "=====================================")
