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
 #include <optional>

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

    // Prevent copying (database connections shouldn't be copied)
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    // ==== DATABASE INITIALIZATION ====

    /**
     * @brief Initialize the database schema
     * 
     * Creates all necessary tables if they don't exist. This is called
     * automatically by the constructor, but can be called manually if needed.
     * 
     * Tables created:
     * - books: Stores book information and reading progress
     * 
     * @throws std::runtime_error if table creation fails
     */
    void initializeSchema();

    // ==== CREATE OPERATIONS ====

    /**
     * @brief Add a new book to the database
     * 
     * @param book Reference to the Book object to add
     * @return The unique ID assigned to the book by the database
     * 
     * This method inserts a new book record into the database. The book's
     * ID will be automatically set by the database (auto-increment).
     * The passed Book object will have its ID updated to match the database ID.
     * 
     * @throws std::runtime_error if insertion fails
     */
    int addBook(Book& book);

    // ==== READ OPERATIONS ====

    /**
     * @brief Retrieve a single book by its ID
     * 
     * @param id The unique identifier of the book
     * @return Book object if found, std::nullopt if not found
     * 
     * Searches the database for a book with the specified ID and
     * returns it as a Book object. If no book is found, returns
     * an empty optional.
     */
    std::optional<Book> getBook(int id) const;

    /**
     * @brief Retrieve all books from the database
     * 
     * @return Vector containing all Book objects in the database
     * 
     * Returns an empty vector if no books exist. Books are returned
     * in order by their ID (oldest first).
     * 
     * @throws std::runtime_error if query fails
     */
    std::vector<Book> getAllBooks() const;

    /**
     * @brief Search for books by title or author
     * 
     * @param query Search string to match against title or author
     * @return Vector of Book objects matching the search query
     * 
     * Performs a case-insensitive partial match search. For example,
     * searching for "lord" will match "The Lord of the Rings".
     * Returns empty vector if no matches found.
     * 
     * @throws std::runtime_error if query fails
     */
    std::vector<Book> searchBooks(const std::string& query) const;

    // ==== UPDATE OPERATIONS ====

    /**
     * @brief Update an existing book in the database
     * 
     * @param book The Book object with updated information
     * @return true if update successful, false if book not found
     * 
     * Updates all fields of the book record matching the book's ID.
     * The book must have a valid ID (> 0) set.
     * 
     * @throws std::runtime_error if update fails
     * @throws std::invalid_argument if book ID is invalid
     */
    bool updateBook(const Book& book);

    // ==== DELETE OPERATIONS ====

    /**
     * @brief Delete a book from the database
     * 
     * @param id The unique identifier of the book to delete
     * @return true if deletion successful, false if book not found
     * 
     * Permanently removes the book record from the database.
     * This operation cannot be undone.
     * 
     * @throws std::runtime_error if deletion fails
     */
    bool deleteBook(int id);

    // ==== TRANSACTION SUPPORT ====

    /**
     * @brief Begin a database transaction
     * 
     * Starts a transaction for grouping multiple operations.
     * Changes won't be saved until commitTransaction() is called.
     * Use rollbackTransaction() to cancel all changes.
     * 
     * @throws std::runtime_error if transaction start fails
     */
    void beginTransaction();

    /**
     * @brief Commit the current transaction
     * 
     * Saves all changes made since beginTransaction() was called.
     * 
     * @throws std::runtime_error if commit fails
     */
    void commitTransaction();

    /**
     * @brief Rollback the current transaction
     * 
     * Discards all changes made since beginTransaction() was called.
     * 
     * @throws std::runtime_error if rollback fails
     */
    void rollbackTransaction();

    // ==== UTILITY METHODS ====

    /**
     * @brief Close the database connection
     * 
     * Closes the connection to the SQLite database file.
     * This is called automatically by the destructor, but can
     * be called manually if needed.
     */
    void close();

    /**
     * @brief Check if database is open and connected
     * 
     * @return true if database connection is active, false otherwise
     */
    bool isOpen() const;

private:
    // ==== MEMBER VARIABLES ====

    sqlite3* m_db;                  ///< SQLite database connection handle
    std::string m_dbPath;           ///< Path to the database file
    bool m_isOpen;                  ///< Connection status flag

    // ==== HELPER METHODS ====

    /**
     * @brief Convert a database row to a Book object
     * 
     * @param stmt Prepared statement positioned at a valid row
     * @return Book object populated from the current row
     * 
     * Internal helper method to extract column values from a SQLite
     * result row and construct a Book object.
     */
    Book bookFromStatement(sqlite3_stmt* stmt) const;

    /**
     * @brief Execute a SQL statement that doesn't return results
     * 
     * @param sql The SQL statement to execute
     * @throws std::runtime_error if execution fails
     * 
     * Used for CREATE, INSERT, UPDATE, DELETE operations.
     */
    void executeSQL(const std::string& sql);
};

#endif // DATABASE_H