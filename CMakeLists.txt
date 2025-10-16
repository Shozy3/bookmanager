# Personal Reading Management System (PRMS)
# CMake configuration for cross-platform C++ application

cmake_minimum_required(VERSION 3.16)

# Project metadata
project(PRMS
    VERSION 1.0.0
    DESCRIPTION "Personal Reading Management System"
    LANGUAGES CXX
)

# Set C++ standard to C++17 for modern features
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler-specific options
if(MSVC)
    # Microsoft Visual C++ specific settings
    add_compile_options(/W4 /permissive-)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
else()
    # GCC/Clang specific settings
    add_compile_options(-Wall -Wextra -Wpedantic)
    add_compile_options(-Wno-unused-parameter)  # Allow unused parameters in interface headers
endif()

# Build type configuration
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Find required packages
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Charts)
find_package(SQLite3 REQUIRED)

# Enable Qt6 automatic MOC (Meta-Object Compiler)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files (we'll add more as we create them)
set(SOURCES
    src/main.cpp
    src/core/book.cpp
    src/core/database.cpp
    src/core/reading_session.cpp
    src/ui/theme.cpp
    src/ui/mainwindow.cpp
    src/ui/dialogs/book_dialog.cpp
)

# Header files (we'll add more as we create them)
set(HEADERS
    include/core/book.h
    include/core/database.h
    include/core/reading_session.h
    include/core/book_status.h
    include/ui/theme.h
    include/ui/mainwindow.h
    include/ui/dialogs/book_dialog.h
    include/utils/date_utils.h
    include/utils/string_utils.h
)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Widgets
    Qt6::Charts
    SQLite::SQLite3
)

# Compiler definitions
target_compile_definitions(${PROJECT_NAME} PRIVATE
    QT_DEPRECATED_WARNINGS
    QT_DISABLE_DEPRECATED_BEFORE=0x060000
)

# Installation rules
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Testing (will be enabled in future commits)
# enable_testing()
# add_subdirectory(tests)

# Documentation (will be enabled in future commits)
# find_package(Doxygen)
# if(DOXYGEN_FOUND)
#     add_subdirectory(docs)
# endif()

# Print configuration summary
message(STATUS "=== PRMS Configuration Summary ===")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Qt version: ${Qt6_VERSION}")
message(STATUS "SQLite version: ${SQLite3_VERSION}")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "=====================================")
