/**
 * @file database.cpp
 * @brief Implementation of the Database class for the Personal Reading Management System (PRMS)
 * 
 * This file contains the complete implementation of database operations using SQLite.
 * It demonstrates how to interact with a relational database from C++.
 * 
 * @author Shahzaib Ahmed
 * @date October 16, 2025
 */

#include "core/database.h"
#include <stdexcept>
#include <sstream>
#include <iostream>
#include <chrono>

// ============================================================================
// EDUCATIONAL NOTE: What is a Database?
// ============================================================================
// A database is a structured way to store data persistently (data survives
// after the program closes). Think of it like a smart filing cabinet:
// - Tables are like folders (e.g., "books", "authors")
// - Rows are like individual documents (e.g., one book)
// - Columns are like fields on a form (e.g., title, author, page count)
//
// SQLite is a "embedded" database - it stores everything in a single file
// on your computer. No server needed!
// ============================================================================

// ============================================================================
// CONSTRUCTOR: Opening the Database Connection
// ============================================================================

/**
 * @brief Constructor - Opens connection to SQLite database
 * 
 * WHAT HAPPENS HERE:
 * 1. Attempts to open the SQLite database file at the specified path
 * 2. If the file doesn't exist, SQLite creates it automatically
 * 3. Initializes the database schema (creates tables if needed)
 * 
 * IMPORTANT CONCEPTS:
 * - sqlite3_open(): Opens a connection to a database file
 * - SQLITE_OK: Return code indicating success
 * - m_db: Pointer to the database connection (handle)
 * - RAII Pattern: Resource is acquired in constructor, released in destructor
 */
Database::Database(const std::string& dbPath)
    : m_db(nullptr)      // Initialize database handle to null
    , m_dbPath(dbPath)   // Store the path to the database file
    , m_isOpen(false)    // Connection starts as closed
{
    // Attempt to open the database file
    // sqlite3_open takes: 1) path to file, 2) pointer to store connection handle
    int result = sqlite3_open(dbPath.c_str(), &m_db);
    
    // Check if opening was successful
    if (result != SQLITE_OK) {
        // Get error message from SQLite before we lose the connection
        std::string errorMsg = sqlite3_errmsg(m_db);
        
        // Clean up the failed connection
        sqlite3_close(m_db);
        m_db = nullptr;
        
        // Throw an exception with a helpful message
        throw std::runtime_error("Failed to open database: " + errorMsg);
    }
    
    // Connection successful!
    m_isOpen = true;
    
    std::cout << "[Database] Opened database: " << dbPath << std::endl;
    
    // Initialize the database schema (create tables if they don't exist)
    initializeSchema();
}

// ============================================================================
// DESTRUCTOR: Closing the Database Connection
// ============================================================================

/**
 * @brief Destructor - Closes the database connection
 * 
 * WHAT HAPPENS HERE:
 * - Automatically called when the Database object is destroyed
 * - Ensures the database connection is properly closed
 * - Releases system resources
 * 
 * IMPORTANT CONCEPT:
 * - RAII (Resource Acquisition Is Initialization): C++ guarantees the
 *   destructor will be called, so we're guaranteed cleanup happens
 */
Database::~Database() {
    close();
}

/**
 * @brief Close the database connection
 */
void Database::close() {
    if (m_db != nullptr) {
        sqlite3_close(m_db);
        m_db = nullptr;
        m_isOpen = false;
        std::cout << "[Database] Closed database connection" << std::endl;
    }
}

/**
 * @brief Check if database is open
 */
bool Database::isOpen() const {
    return m_isOpen && m_db != nullptr;
}

// ============================================================================
// SCHEMA INITIALIZATION: Creating Tables
// ============================================================================

/**
 * @brief Initialize database schema - Create tables if they don't exist
 * 
 * EDUCATIONAL NOTE: What is SQL?
 * SQL (Structured Query Language) is the language we use to talk to databases.
 * Think of it like giving instructions to a librarian:
 * - CREATE TABLE: "Create a new filing system"
 * - INSERT: "Add a new book to the shelf"
 * - SELECT: "Find and show me some books"
 * - UPDATE: "Change the information for a book"
 * - DELETE: "Remove a book from the collection"
 * 
 * ABOUT THIS TABLE:
 * - id: Unique number for each book (auto-generated)
 * - title, author, isbn: Book information
 * - page_count, current_page: For tracking progress
 * - start_date, completion_date: When reading started/finished
 * 
 * DATA TYPES:
 * - INTEGER: Whole numbers (1, 2, 100, -5)
 * - TEXT: Strings of characters ("Hello", "Book Title")
 * - REAL: Decimal numbers (3.14, 99.99)
 * 
 * CONSTRAINTS:
 * - PRIMARY KEY: Unique identifier for each row
 * - AUTOINCREMENT: Database automatically assigns next number
 * - NOT NULL: This field is required (can't be empty)
 * - DEFAULT: If no value provided, use this
 */
void Database::initializeSchema() {
    // SQL to create the books table with all fields
    const char* createBooksTableSQL = R"(
        CREATE TABLE IF NOT EXISTS books (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            title TEXT NOT NULL,
            author TEXT NOT NULL,
            isbn TEXT,
            page_count INTEGER NOT NULL DEFAULT 0,
            current_page INTEGER NOT NULL DEFAULT 0,
            start_date INTEGER,
            completion_date INTEGER,
            genre TEXT,
            publisher TEXT,
            year_published INTEGER,
            notes TEXT,
            review TEXT,
            rating INTEGER DEFAULT 0,
            cover_path TEXT,
            date_added INTEGER NOT NULL,
            status INTEGER DEFAULT 0
        );
    )";
    
    // SQL to create reading sessions table
    const char* createSessionsTableSQL = R"(
        CREATE TABLE IF NOT EXISTS reading_sessions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            book_id INTEGER NOT NULL,
            session_date INTEGER NOT NULL,
            duration_minutes INTEGER NOT NULL,
            pages_read INTEGER NOT NULL,
            start_page INTEGER NOT NULL,
            end_page INTEGER NOT NULL,
            notes TEXT,
            FOREIGN KEY (book_id) REFERENCES books(id) ON DELETE CASCADE
        );
    )";
    
    // SQL to create API cache table for future use
    const char* createCacheTableSQL = R"(
        CREATE TABLE IF NOT EXISTS api_cache (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            cache_key TEXT UNIQUE NOT NULL,
            cache_data TEXT NOT NULL,
            created_at INTEGER NOT NULL,
            expires_at INTEGER NOT NULL
        );
    )";
    
    // Execute all schema creation statements
    executeSQL(createBooksTableSQL);
    executeSQL(createSessionsTableSQL);
    executeSQL(createCacheTableSQL);
    
    std::cout << "[Database] Schema initialized successfully" << std::endl;
}

// ============================================================================
// HELPER METHOD: Execute Simple SQL
// ============================================================================

/**
 * @brief Execute a SQL statement that doesn't return data
 * 
 * WHEN TO USE:
 * - CREATE TABLE, INSERT, UPDATE, DELETE
 * - Any SQL that changes data but doesn't return results
 * 
 * HOW IT WORKS:
 * - sqlite3_exec(): Simple way to run SQL when you don't need results
 * - Takes: connection, SQL string, callback (we use nullptr), data, error message
 */
void Database::executeSQL(const std::string& sql) {
    char* errorMessage = nullptr;
    
    // Execute the SQL
    int result = sqlite3_exec(m_db, sql.c_str(), nullptr, nullptr, &errorMessage);
    
    // Check for errors
    if (result != SQLITE_OK) {
        std::string error = errorMessage ? errorMessage : "Unknown error";
        sqlite3_free(errorMessage);
        throw std::runtime_error("SQL execution failed: " + error);
    }
}

// ============================================================================
// CREATE OPERATION: Adding a Book (INSERT)
// ============================================================================

/**
 * @brief Add a new book to the database
 * 
 * EDUCATIONAL NOTE: Prepared Statements
 * Instead of building SQL by concatenating strings (dangerous!), we use
 * "prepared statements" which are safer and faster:
 * 
 * BAD (vulnerable to SQL injection):
 *   sql = "INSERT INTO books VALUES ('" + title + "', '" + author + "')";
 * 
 * GOOD (safe with prepared statements):
 *   sql = "INSERT INTO books VALUES (?, ?)";
 *   Then we "bind" the values to replace the ? placeholders
 * 
 * STEPS:
 * 1. Prepare the SQL statement (compile it)
 * 2. Bind values to replace ? placeholders
 * 3. Execute the statement
 * 4. Get the auto-generated ID
 * 5. Clean up (finalize the statement)
 * 
 * BINDING INDEXES:
 * - SQL parameters are 1-based (first ? is index 1, not 0!)
 */
int Database::addBook(Book& book) {
    // SQL with ? placeholders for all fields
    const char* sql = R"(
        INSERT INTO books (
            title, author, isbn, page_count, current_page, 
            start_date, completion_date, genre, publisher, year_published,
            notes, review, rating, cover_path, date_added, status
        )
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);
    )";
    
    sqlite3_stmt* stmt = nullptr;
    
    // Prepare the statement
    int result = sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare INSERT statement: " + 
                                 std::string(sqlite3_errmsg(m_db)));
    }
    
    // Bind all values
    sqlite3_bind_text(stmt, 1, book.getTitle().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, book.getAuthor().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, book.getISBN().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 4, book.getPageCount());
    sqlite3_bind_int(stmt, 5, book.getCurrentPage());
    
    // Bind start date
    if (book.getStartDate().has_value()) {
        auto timestamp = std::chrono::system_clock::to_time_t(book.getStartDate().value());
        sqlite3_bind_int64(stmt, 6, timestamp);
    } else {
        sqlite3_bind_null(stmt, 6);
    }
    
    // Bind completion date
    if (book.getCompletionDate().has_value()) {
        auto timestamp = std::chrono::system_clock::to_time_t(book.getCompletionDate().value());
        sqlite3_bind_int64(stmt, 7, timestamp);
    } else {
        sqlite3_bind_null(stmt, 7);
    }
    
    // Bind additional fields
    sqlite3_bind_text(stmt, 8, book.getGenre().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 9, book.getPublisher().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 10, book.getYearPublished());
    sqlite3_bind_text(stmt, 11, book.getNotes().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 12, book.getReview().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 13, book.getRating());
    sqlite3_bind_text(stmt, 14, book.getCoverPath().c_str(), -1, SQLITE_TRANSIENT);
    
    // Bind date added
    auto dateAddedTimestamp = std::chrono::system_clock::to_time_t(book.getDateAdded());
    sqlite3_bind_int64(stmt, 15, dateAddedTimestamp);
    
    sqlite3_bind_int(stmt, 16, book.getStatus());
    
    // Execute
    result = sqlite3_step(stmt);
    
    if (result != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to insert book: " + 
                                 std::string(sqlite3_errmsg(m_db)));
    }
    
    // Get auto-generated ID
    int newId = static_cast<int>(sqlite3_last_insert_rowid(m_db));
    book.setId(newId);
    
    sqlite3_finalize(stmt);
    
    std::cout << "[Database] Added book: " << book.getTitle() 
              << " (ID: " << newId << ")" << std::endl;
    
    return newId;
}

// ============================================================================
// READ OPERATIONS: Retrieving Books (SELECT)
// ============================================================================

/**
 * @brief Retrieve a single book by ID
 * 
 * EDUCATIONAL NOTE: Reading Data
 * When we SELECT data from a database, we get back a "result set" which
 * is like a temporary table of results. We use sqlite3_step() to move
 * through the rows one at a time.
 * 
 * RETURN VALUES from sqlite3_step():
 * - SQLITE_ROW: We got a row of data
 * - SQLITE_DONE: No more rows (query finished)
 * - Other: An error occurred
 */
std::optional<Book> Database::getBook(int id) const {
    const char* sql = R"(
        SELECT id, title, author, isbn, page_count, current_page, 
               start_date, completion_date, genre, publisher, year_published,
               notes, review, rating, cover_path, date_added, status 
        FROM books WHERE id = ?;
    )";
    
    sqlite3_stmt* stmt = nullptr;
    
    // Prepare the statement
    int result = sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare SELECT statement: " + 
                                 std::string(sqlite3_errmsg(m_db)));
    }
    
    // Bind the ID parameter
    sqlite3_bind_int(stmt, 1, id);
    
    // Execute and check if we got a row
    result = sqlite3_step(stmt);
    
    if (result == SQLITE_ROW) {
        // We found the book! Convert the row to a Book object
        Book book = bookFromStatement(stmt);
        sqlite3_finalize(stmt);
        return book;  // Return the book wrapped in optional
    } else if (result == SQLITE_DONE) {
        // No rows found - book doesn't exist
        sqlite3_finalize(stmt);
        return std::nullopt;  // Return empty optional
    } else {
        // An error occurred
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to retrieve book: " + 
                                 std::string(sqlite3_errmsg(m_db)));
    }
}

/**
 * @brief Retrieve all books from the database
 * 
 * EDUCATIONAL NOTE: Processing Multiple Rows
 * We call sqlite3_step() in a loop to process each row:
 * - Keep calling sqlite3_step() while it returns SQLITE_ROW
 * - When it returns SQLITE_DONE, we've processed all rows
 */
std::vector<Book> Database::getAllBooks() const {
    const char* sql = R"(
        SELECT id, title, author, isbn, page_count, current_page, 
               start_date, completion_date, genre, publisher, year_published,
               notes, review, rating, cover_path, date_added, status 
        FROM books ORDER BY id;
    )";
    
    sqlite3_stmt* stmt = nullptr;
    std::vector<Book> books;
    
    // Prepare the statement
    int result = sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare SELECT ALL statement: " + 
                                 std::string(sqlite3_errmsg(m_db)));
    }
    
    // Loop through all rows
    while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
        // Convert each row to a Book and add to vector
        books.push_back(bookFromStatement(stmt));
    }
    
    // Check if loop ended due to error or normal completion
    if (result != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to retrieve books: " + 
                                 std::string(sqlite3_errmsg(m_db)));
    }
    
    sqlite3_finalize(stmt);
    
    std::cout << "[Database] Retrieved " << books.size() << " books" << std::endl;
    
    return books;
}

/**
 * @brief Search for books by title or author
 * 
 * EDUCATIONAL NOTE: The LIKE Operator
 * LIKE allows pattern matching in SQL:
 * - % matches any number of characters (including zero)
 * - _ matches exactly one character
 * 
 * Examples:
 * - "The%" matches "The Lord of the Rings", "The Hobbit"
 * - "%lord%" matches "The Lord of the Rings", "Overlord"
 * - "The ___" matches "The End" but not "The Beginning"
 * 
 * We use LOWER() to make the search case-insensitive
 */
std::vector<Book> Database::searchBooks(const std::string& query) const {
    const char* sql = R"(
        SELECT id, title, author, isbn, page_count, current_page, 
               start_date, completion_date, genre, publisher, year_published,
               notes, review, rating, cover_path, date_added, status 
        FROM books 
        WHERE LOWER(title) LIKE LOWER(?) OR LOWER(author) LIKE LOWER(?)
        ORDER BY title;
    )";
    
    sqlite3_stmt* stmt = nullptr;
    std::vector<Book> books;
    
    // Prepare the statement
    int result = sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare SEARCH statement: " + 
                                 std::string(sqlite3_errmsg(m_db)));
    }
    
    // Add % wildcards to search query to match anywhere in the text
    std::string searchPattern = "%" + query + "%";
    
    // Bind the same pattern to both placeholders (title and author)
    sqlite3_bind_text(stmt, 1, searchPattern.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, searchPattern.c_str(), -1, SQLITE_TRANSIENT);
    
    // Loop through results
    while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
        books.push_back(bookFromStatement(stmt));
    }
    
    if (result != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to search books: " + 
                                 std::string(sqlite3_errmsg(m_db)));
    }
    
    sqlite3_finalize(stmt);
    
    std::cout << "[Database] Search for '" << query << "' found " 
              << books.size() << " books" << std::endl;
    
    return books;
}

/**
 * @brief Helper method: Convert database row to Book object
 * 
 * EDUCATIONAL NOTE: Reading Column Values
 * sqlite3_column_*() functions extract values from the current row:
 * - Column indexes are 0-based (first column is 0)
 * - Type must match what you're reading (int, text, etc.)
 * 
 * CHECKING FOR NULL:
 * - sqlite3_column_type() returns SQLITE_NULL if column is NULL
 * - Important for optional fields like dates
 */
Book Database::bookFromStatement(sqlite3_stmt* stmt) const {
    // Create a default Book object
    Book book;
    
    // Read each column from the result row
    // Column 0: id
    book.setId(sqlite3_column_int(stmt, 0));
    
    // Column 1: title
    const unsigned char* title = sqlite3_column_text(stmt, 1);
    book.setTitle(title ? reinterpret_cast<const char*>(title) : "");
    
    // Column 2: author
    const unsigned char* author = sqlite3_column_text(stmt, 2);
    book.setAuthor(author ? reinterpret_cast<const char*>(author) : "");
    
    // Column 3: isbn
    const unsigned char* isbn = sqlite3_column_text(stmt, 3);
    if (isbn) {
        book.setISBN(reinterpret_cast<const char*>(isbn));
    }
    
    // Column 4: page_count
    book.setPageCount(sqlite3_column_int(stmt, 4));
    
    // Column 5: current_page
    book.setCurrentPage(sqlite3_column_int(stmt, 5));
    
    // Column 6: start_date (may be NULL)
    if (sqlite3_column_type(stmt, 6) != SQLITE_NULL) {
        int64_t timestamp = sqlite3_column_int64(stmt, 6);
        auto timePoint = std::chrono::system_clock::from_time_t(timestamp);
        book.setStartDate(timePoint);
    }
    
    // Column 7: completion_date (may be NULL)
    if (sqlite3_column_type(stmt, 7) != SQLITE_NULL) {
        int64_t timestamp = sqlite3_column_int64(stmt, 7);
        auto timePoint = std::chrono::system_clock::from_time_t(timestamp);
        book.setCompletionDate(timePoint);
    }
    
    // Column 8: genre
    const unsigned char* genre = sqlite3_column_text(stmt, 8);
    if (genre) {
        book.setGenre(reinterpret_cast<const char*>(genre));
    }
    
    // Column 9: publisher
    const unsigned char* publisher = sqlite3_column_text(stmt, 9);
    if (publisher) {
        book.setPublisher(reinterpret_cast<const char*>(publisher));
    }
    
    // Column 10: year_published
    book.setYearPublished(sqlite3_column_int(stmt, 10));
    
    // Column 11: notes
    const unsigned char* notes = sqlite3_column_text(stmt, 11);
    if (notes) {
        book.setNotes(reinterpret_cast<const char*>(notes));
    }
    
    // Column 12: review
    const unsigned char* review = sqlite3_column_text(stmt, 12);
    if (review) {
        book.setReview(reinterpret_cast<const char*>(review));
    }
    
    // Column 13: rating
    book.setRating(sqlite3_column_int(stmt, 13));
    
    // Column 14: cover_path
    const unsigned char* coverPath = sqlite3_column_text(stmt, 14);
    if (coverPath) {
        book.setCoverPath(reinterpret_cast<const char*>(coverPath));
    }
    
    // Column 15: date_added
    if (sqlite3_column_type(stmt, 15) != SQLITE_NULL) {
        int64_t timestamp = sqlite3_column_int64(stmt, 15);
        auto timePoint = std::chrono::system_clock::from_time_t(timestamp);
        book.setDateAdded(timePoint);
    }
    
    // Column 16: status
    book.setStatus(sqlite3_column_int(stmt, 16));
    
    return book;
}

// ============================================================================
// UPDATE OPERATION: Modifying a Book
// ============================================================================

/**
 * @brief Update an existing book in the database
 * 
 * EDUCATIONAL NOTE: The WHERE Clause
 * The WHERE clause is CRITICAL in UPDATE statements!
 * - Without WHERE, you'd update EVERY row in the table
 * - WHERE id = ? ensures we only update the specific book
 * 
 * CHECKING AFFECTED ROWS:
 * - sqlite3_changes() tells us how many rows were modified
 * - If 0 rows changed, the book didn't exist
 */
bool Database::updateBook(const Book& book) {
    // Validate that the book has an ID
    if (book.getId() <= 0) {
        throw std::invalid_argument("Cannot update book: invalid ID");
    }
    
    const char* sql = R"(
        UPDATE books 
        SET title = ?, author = ?, isbn = ?, page_count = ?, 
            current_page = ?, start_date = ?, completion_date = ?,
            genre = ?, publisher = ?, year_published = ?,
            notes = ?, review = ?, rating = ?, cover_path = ?,
            date_added = ?, status = ?
        WHERE id = ?;
    )";
    
    sqlite3_stmt* stmt = nullptr;
    
    // Prepare statement
    int result = sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare UPDATE statement: " + 
                                 std::string(sqlite3_errmsg(m_db)));
    }
    
    // Bind all values
    sqlite3_bind_text(stmt, 1, book.getTitle().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, book.getAuthor().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 3, book.getISBN().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 4, book.getPageCount());
    sqlite3_bind_int(stmt, 5, book.getCurrentPage());
    
    // Bind dates
    if (book.getStartDate().has_value()) {
        auto timestamp = std::chrono::system_clock::to_time_t(book.getStartDate().value());
        sqlite3_bind_int64(stmt, 6, timestamp);
    } else {
        sqlite3_bind_null(stmt, 6);
    }
    
    if (book.getCompletionDate().has_value()) {
        auto timestamp = std::chrono::system_clock::to_time_t(book.getCompletionDate().value());
        sqlite3_bind_int64(stmt, 7, timestamp);
    } else {
        sqlite3_bind_null(stmt, 7);
    }
    
    // Bind additional fields
    sqlite3_bind_text(stmt, 8, book.getGenre().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 9, book.getPublisher().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 10, book.getYearPublished());
    sqlite3_bind_text(stmt, 11, book.getNotes().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 12, book.getReview().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 13, book.getRating());
    sqlite3_bind_text(stmt, 14, book.getCoverPath().c_str(), -1, SQLITE_TRANSIENT);
    
    // Bind date added
    auto dateAddedTimestamp = std::chrono::system_clock::to_time_t(book.getDateAdded());
    sqlite3_bind_int64(stmt, 15, dateAddedTimestamp);
    
    sqlite3_bind_int(stmt, 16, book.getStatus());
    
    // Bind the ID for the WHERE clause
    sqlite3_bind_int(stmt, 17, book.getId());
    
    // Execute
    result = sqlite3_step(stmt);
    
    if (result != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to update book: " + 
                                 std::string(sqlite3_errmsg(m_db)));
    }
    
    // Check how many rows were affected
    int rowsChanged = sqlite3_changes(m_db);
    
    sqlite3_finalize(stmt);
    
    if (rowsChanged > 0) {
        std::cout << "[Database] Updated book ID " << book.getId() << std::endl;
        return true;
    } else {
        std::cout << "[Database] Book ID " << book.getId() << " not found" << std::endl;
        return false;
    }
}

// ============================================================================
// DELETE OPERATION: Removing a Book
// ============================================================================

/**
 * @brief Delete a book from the database
 * 
 * EDUCATIONAL NOTE: DELETE is Permanent!
 * - Once deleted, data is gone (unless you have backups)
 * - Always use WHERE clause to specify which row(s) to delete
 * - Consider "soft delete" (marking as deleted) for important data
 */
bool Database::deleteBook(int id) {
    const char* sql = "DELETE FROM books WHERE id = ?;";
    
    sqlite3_stmt* stmt = nullptr;
    
    // Prepare statement
    int result = sqlite3_prepare_v2(m_db, sql, -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare DELETE statement: " + 
                                 std::string(sqlite3_errmsg(m_db)));
    }
    
    // Bind the ID
    sqlite3_bind_int(stmt, 1, id);
    
    // Execute
    result = sqlite3_step(stmt);
    
    if (result != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to delete book: " + 
                                 std::string(sqlite3_errmsg(m_db)));
    }
    
    // Check if a row was deleted
    int rowsDeleted = sqlite3_changes(m_db);
    
    sqlite3_finalize(stmt);
    
    if (rowsDeleted > 0) {
        std::cout << "[Database] Deleted book ID " << id << std::endl;
        return true;
    } else {
        std::cout << "[Database] Book ID " << id << " not found" << std::endl;
        return false;
    }
}

// ============================================================================
// TRANSACTION SUPPORT: Grouping Multiple Operations
// ============================================================================

/**
 * @brief Begin a database transaction
 * 
 * EDUCATIONAL NOTE: What are Transactions?
 * A transaction groups multiple operations together as one unit:
 * - Either ALL operations succeed, or NONE do
 * - Example: Transferring money between bank accounts
 *   1. Subtract from Account A
 *   2. Add to Account B
 *   If step 2 fails, we rollback step 1 (can't lose money!)
 * 
 * ACID PROPERTIES:
 * - Atomicity: All or nothing
 * - Consistency: Database stays in valid state
 * - Isolation: Transactions don't interfere with each other
 * - Durability: Committed data is saved permanently
 */
void Database::beginTransaction() {
    executeSQL("BEGIN TRANSACTION;");
    std::cout << "[Database] Transaction started" << std::endl;
}

/**
 * @brief Commit the current transaction (save changes)
 */
void Database::commitTransaction() {
    executeSQL("COMMIT;");
    std::cout << "[Database] Transaction committed" << std::endl;
}

/**
 * @brief Rollback the current transaction (discard changes)
 */
void Database::rollbackTransaction() {
    executeSQL("ROLLBACK;");
    std::cout << "[Database] Transaction rolled back" << std::endl;
}

