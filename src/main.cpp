/**
 * @file main.cpp
 * @brief Main entry point for the Personal Reading Management System (PRMS)
 * 
 * This file contains the main function that initializes the Qt application
 * and starts the GUI. It serves as the entry point for the entire application.
 * 
 * @author Shahzaib Ahmed
 * @date October 12, 2025
 */

#include <QApplication>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>
#include "ui/mainwindow.h"
#include "ui/theme.h"


/**
 * @brief Creates necessary application directories
 * 
 * This function ensures that all required directories exist for the application
 * to function properly. It creates directories for data storage, configuration,
 * and temporary files in the appropriate system locations.
 */
void createApplicationDirectories() {
    // Get application data directory
    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString configDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    
    // Create directories if they don't exist
    QDir().mkpath(dataDir);
    QDir().mkpath(configDir);
    
    qDebug() << "Application data directory:" << dataDir;
    qDebug() << "Application config directory:" << configDir;
}

// Old database test code removed - functionality now in MainWindow
/* void testDatabaseOperations() {
    std::cout << "\n========================================" << std::endl;
    std::cout << "DATABASE OPERATIONS TUTORIAL" << std::endl;
    std::cout << "========================================\n" << std::endl;
    
    try {
        // ====================================================================
        // LESSON 1: Creating a Database Connection
        // ====================================================================
        std::cout << "LESSON 1: Creating Database Connection\n" << std::endl;
        std::cout << "What happens: SQLite creates (or opens) a file called" << std::endl;
        std::cout << "'prms_test.db' which stores all our data." << std::endl;
        std::cout << "If the file doesn't exist, it's created automatically.\n" << std::endl;
        
        Database db("prms_test.db");
        std::cout << "✓ Database connected successfully!\n" << std::endl;
        
        // ====================================================================
        // LESSON 2: Adding Books (INSERT Operation)
        // ====================================================================
        std::cout << "LESSON 2: Adding Books to Database\n" << std::endl;
        std::cout << "We'll create Book objects and save them to the database." << std::endl;
        std::cout << "The database will automatically assign each book a unique ID.\n" << std::endl;
        
        // Create first book
        Book book1("The Hobbit", "J.R.R. Tolkien", "9780547928227", 310);
        std::cout << "Created book: " << book1.getTitle() << std::endl;
        std::cout << "Book ID before saving: " << book1.getId() << " (not set yet)" << std::endl;
        
        int id1 = db.addBook(book1);
        std::cout << "Book ID after saving: " << book1.getId() << " (auto-assigned!)" << std::endl;
        std::cout << "✓ Book 1 added with ID: " << id1 << "\n" << std::endl;
        
        // Create second book
        Book book2("1984", "George Orwell", "9780451524935", 328);
        int id2 = db.addBook(book2);
        std::cout << "✓ Book 2 added with ID: " << id2 << "\n" << std::endl;
        
        // Create third book with reading progress
        Book book3("Dune", "Frank Herbert", "9780441172719", 688);
        book3.setCurrentPage(150);  // Already read 150 pages
        int id3 = db.addBook(book3);
        std::cout << "✓ Book 3 added with ID: " << id3 << " (with progress: " 
                  << book3.getProgressPercentage() << "%)\n" << std::endl;
        
        // ====================================================================
        // LESSON 3: Retrieving a Single Book (SELECT with WHERE)
        // ====================================================================
        std::cout << "LESSON 3: Retrieving a Specific Book\n" << std::endl;
        std::cout << "We'll fetch a book by its ID using getBook().\n" << std::endl;
        
        auto retrievedBook = db.getBook(id1);
        if (retrievedBook.has_value()) {
            std::cout << "✓ Found book ID " << id1 << ":" << std::endl;
            std::cout << "  Title: " << retrievedBook->getTitle() << std::endl;
            std::cout << "  Author: " << retrievedBook->getAuthor() << std::endl;
            std::cout << "  ISBN: " << retrievedBook->getISBN() << std::endl;
            std::cout << "  Pages: " << retrievedBook->getPageCount() << std::endl;
        } else {
            std::cout << "✗ Book not found!" << std::endl;
        }
        std::cout << std::endl;
        
        // Try to get a non-existent book
        auto missingBook = db.getBook(9999);
        if (!missingBook.has_value()) {
            std::cout << "✓ Correctly returned 'not found' for ID 9999\n" << std::endl;
        }
        
        // ====================================================================
        // LESSON 4: Retrieving All Books (SELECT without WHERE)
        // ====================================================================
        std::cout << "LESSON 4: Retrieving All Books\n" << std::endl;
        std::cout << "We'll fetch every book in the database using getAllBooks().\n" << std::endl;
        
        std::vector<Book> allBooks = db.getAllBooks();
        std::cout << "✓ Retrieved " << allBooks.size() << " books:\n" << std::endl;
        
        for (const auto& book : allBooks) {
            std::cout << "  [ID " << book.getId() << "] " 
                      << book.getTitle() << " by " << book.getAuthor();
            if (book.getCurrentPage() > 0) {
                std::cout << " (Progress: " << book.getProgressPercentage() << "%)";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
        
        // ====================================================================
        // LESSON 5: Updating a Book (UPDATE Operation)
        // ====================================================================
        std::cout << "LESSON 5: Updating Reading Progress\n" << std::endl;
        std::cout << "We'll update a book's reading progress and save it back.\n" << std::endl;
        
        if (retrievedBook.has_value()) {
            std::cout << "Before update - Current page: " 
                      << retrievedBook->getCurrentPage() << std::endl;
            
            // Update reading progress
            retrievedBook->setCurrentPage(100);
            std::cout << "After update - Current page: " 
                      << retrievedBook->getCurrentPage() 
                      << " (Progress: " << retrievedBook->getProgressPercentage() << "%)" 
                      << std::endl;
            
            // Save changes to database
            bool updated = db.updateBook(retrievedBook.value());
            if (updated) {
                std::cout << "✓ Book updated in database!" << std::endl;
                
                // Verify the update by retrieving again
                auto verifyBook = db.getBook(id1);
                std::cout << "✓ Verified - Current page is now: " 
                          << verifyBook->getCurrentPage() << "\n" << std::endl;
            }
        }
        
        // ====================================================================
        // LESSON 6: Searching Books (SELECT with LIKE)
        // ====================================================================
        std::cout << "LESSON 6: Searching for Books\n" << std::endl;
        std::cout << "We'll search for books by title or author using partial matches.\n" << std::endl;
        
        std::cout << "Search query: 'dune'\n" << std::endl;
        std::vector<Book> searchResults = db.searchBooks("dune");
        std::cout << "✓ Found " << searchResults.size() << " matching book(s):" << std::endl;
        for (const auto& book : searchResults) {
            std::cout << "  - " << book.getTitle() << " by " << book.getAuthor() << std::endl;
        }
        std::cout << std::endl;
        
        std::cout << "Search query: 'tolkien' (searching by author)\n" << std::endl;
        searchResults = db.searchBooks("tolkien");
        std::cout << "✓ Found " << searchResults.size() << " matching book(s):" << std::endl;
        for (const auto& book : searchResults) {
            std::cout << "  - " << book.getTitle() << " by " << book.getAuthor() << std::endl;
        }
        std::cout << std::endl;
        
        // ====================================================================
        // LESSON 7: Transactions (Grouping Multiple Operations)
        // ====================================================================
        std::cout << "LESSON 7: Using Transactions\n" << std::endl;
        std::cout << "Transactions let us group multiple operations together." << std::endl;
        std::cout << "Either ALL operations succeed, or NONE do.\n" << std::endl;
        
        db.beginTransaction();
        std::cout << "Transaction started..." << std::endl;
        
        // Add a book within transaction
        Book book4("Foundation", "Isaac Asimov", "9780553293357", 255);
        int id4 = db.addBook(book4);
        std::cout << "  Added book within transaction (ID: " << id4 << ")" << std::endl;
        
        // Commit the transaction (save the changes)
        db.commitTransaction();
        std::cout << "✓ Transaction committed - changes saved!\n" << std::endl;
        
        // Demonstrate rollback
        std::cout << "Now testing rollback (undo changes)...\n" << std::endl;
        db.beginTransaction();
        std::cout << "Transaction started..." << std::endl;
        
        Book book5("Test Book to Rollback", "Test Author", "", 100);
        int id5 = db.addBook(book5);
        std::cout << "  Added temporary book (ID: " << id5 << ")" << std::endl;
        
        // Rollback instead of commit
        db.rollbackTransaction();
        std::cout << "✓ Transaction rolled back - book NOT saved!" << std::endl;
        
        // Verify the book was not saved
        auto rolledBackBook = db.getBook(id5);
        if (!rolledBackBook.has_value()) {
            std::cout << "✓ Verified - rolled back book does not exist\n" << std::endl;
        }
        
        // ====================================================================
        // LESSON 8: Deleting a Book (DELETE Operation)
        // ====================================================================
        std::cout << "LESSON 8: Deleting a Book\n" << std::endl;
        std::cout << "We'll permanently remove a book from the database.\n" << std::endl;
        
        std::cout << "Before deletion - Total books: " << db.getAllBooks().size() << std::endl;
        
        bool deleted = db.deleteBook(id2);
        if (deleted) {
            std::cout << "✓ Book ID " << id2 << " deleted successfully!" << std::endl;
        }
        
        std::cout << "After deletion - Total books: " << db.getAllBooks().size() << std::endl;
        
        // Verify book is gone
        auto deletedBook = db.getBook(id2);
        if (!deletedBook.has_value()) {
            std::cout << "✓ Verified - deleted book no longer exists\n" << std::endl;
        }
        
        // ====================================================================
        // FINAL SUMMARY
        // ====================================================================
        std::cout << "========================================" << std::endl;
        std::cout << "DATABASE TUTORIAL COMPLETE!" << std::endl;
        std::cout << "========================================\n" << std::endl;
        
        std::cout << "What you learned:" << std::endl;
        std::cout << "✓ How to create a database connection" << std::endl;
        std::cout << "✓ INSERT - Adding new books" << std::endl;
        std::cout << "✓ SELECT - Retrieving books by ID or all books" << std::endl;
        std::cout << "✓ UPDATE - Modifying existing book data" << std::endl;
        std::cout << "✓ SEARCH - Finding books with LIKE operator" << std::endl;
        std::cout << "✓ DELETE - Removing books" << std::endl;
        std::cout << "✓ TRANSACTIONS - Grouping operations with commit/rollback" << std::endl;
        std::cout << "\nFinal book count: " << db.getAllBooks().size() << " books\n" << std::endl;
        
        // Display all remaining books
        std::cout << "Final book list:" << std::endl;
        for (const auto& book : db.getAllBooks()) {
            std::cout << "  [" << book.getId() << "] " 
                      << book.getTitle() << " by " << book.getAuthor() << std::endl;
        }
        std::cout << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
    }
} */

/**
 * @brief Main entry point of the application
 * 
 * This function initializes the Qt application, sets up the theme,
 * creates necessary directories, and shows the main window.
 * 
 * @param argc Number of command line arguments
 * @param argv Array of command line arguments
 * @return Exit code (0 for success, non-zero for error)
 */
int main(int argc, char *argv[]) {
    // Create Qt application instance
    QApplication app(argc, argv);
    
    // Set application metadata
    app.setApplicationName("Personal Reading Management System");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("PRMS");
    app.setOrganizationDomain("prms.local");
    
    // Set modern Fusion style as base
    app.setStyle("Fusion");
    
    // Create necessary directories
    createApplicationDirectories();
    
    // Print startup information
    qDebug() << "=== PRMS Starting ===";
    qDebug() << "Application Name:" << app.applicationName();
    qDebug() << "Application Version:" << app.applicationVersion();
    qDebug() << "Qt Version:" << QT_VERSION_STR;
    
    // Create and show main window
    MainWindow mainWindow;
    mainWindow.show();
    
    qDebug() << "=== PRMS Ready ===";
    
    // Start the application event loop
    return app.exec();
}
