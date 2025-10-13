/**
 * @file database.h
 * @brief Database management class for the Personal Reading Management System (PRMS)
 * 
 * This class handles all database operations including connection management,
 * table creation, and CRUD operations for Book objects.
 * 
 * @author Shahzaib Ahmed
 * @date October 12, 2025
 */

 #ifndef DATABASE_H
 #define DATABASE_H

 #include "book.h"
 #include <sqlite3.h>
 #include <vector>
 #include <string>

/**
 * @brief Manages databse operations for the PRMS application
 *
 * This class provides a high-level interface to SQLite3 database operations.
 * It handles connection management, table creation, and previous methods
 * to save, load, update, and delete Book objects from the database.
 *
 * Key responsibilities:
 * - Establish and maintain database connection
 * - Create and manage database schema
 * - Convert between Book objects and database rows
 * - Handle database errors gracefully
 * - Provide transaction support for data integrity
 */
 class Database {
    public:

    // ==== CONSTRUCTOR and DESTRUCTOR ====

    /**
     * @brief Constructor - initializes database connection
     * 
     * @param dbPath Path to the SQLite database file
     * 
     * Opens a connection to the SQLite database file. If the file doesn't
     * exist, SQLite will create it. If the file exists but is corrupted,
     * an exception will be thrown.
     */

    explicit Database(const std::string& dbPath);

    /**
     * @brief Destructor - closes database connection
     * 
     * Automatically closes the databse connection when the Database
     * object is destroued. This ensures proper cleanup of resources.
     */
    ~Database();

    // ==== DATABASE INITIALIZATION ====

    
 }

    