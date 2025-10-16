# Database Class Quick Reference

## Quick Start

```cpp
#include "core/database.h"
#include "core/book.h"

// Create/open database
Database db("my_books.db");

// Add a book
Book book("Title", "Author", "1234567890123", 300);
int id = db.addBook(book);  // Returns auto-generated ID

// Get a book
auto maybeBook = db.getBook(id);
if (maybeBook.has_value()) {
    Book& b = maybeBook.value();
    std::cout << b.getTitle() << std::endl;
}

// Update a book
book.setCurrentPage(50);
db.updateBook(book);

// Delete a book
db.deleteBook(id);
```

## Method Reference

### Create

```cpp
int addBook(Book& book)
```
- Inserts new book into database
- Auto-generates and assigns ID to book
- Returns the new ID
- Throws `std::runtime_error` on failure

**Example:**
```cpp
Book book("Dune", "Frank Herbert", "9780441172719", 688);
int id = db.addBook(book);
std::cout << "Book added with ID: " << id << std::endl;
std::cout << "Book object now has ID: " << book.getId() << std::endl;
```

### Read

```cpp
std::optional<Book> getBook(int id) const
```
- Retrieves single book by ID
- Returns `std::nullopt` if not found
- Returns `std::optional<Book>` containing the book if found

**Example:**
```cpp
auto result = db.getBook(5);
if (result.has_value()) {
    Book book = result.value();
    std::cout << "Found: " << book.getTitle() << std::endl;
} else {
    std::cout << "Book not found" << std::endl;
}
```

```cpp
std::vector<Book> getAllBooks() const
```
- Retrieves all books from database
- Returns empty vector if no books exist
- Books ordered by ID

**Example:**
```cpp
std::vector<Book> allBooks = db.getAllBooks();
std::cout << "Total books: " << allBooks.size() << std::endl;
for (const auto& book : allBooks) {
    std::cout << book.getTitle() << std::endl;
}
```

```cpp
std::vector<Book> searchBooks(const std::string& query) const
```
- Searches title and author fields
- Case-insensitive partial matching
- Returns all matches

**Example:**
```cpp
// Find all books with "lord" in title or author
auto results = db.searchBooks("lord");

// Find all books by Tolkien
auto tolkienBooks = db.searchBooks("tolkien");

// Empty query returns empty vector
auto nothing = db.searchBooks("");
```

### Update

```cpp
bool updateBook(const Book& book)
```
- Updates existing book in database
- Book must have valid ID set
- Returns `true` if updated, `false` if not found
- Throws if book has invalid ID (≤ 0)

**Example:**
```cpp
auto book = db.getBook(1);
if (book.has_value()) {
    book->setCurrentPage(100);
    if (db.updateBook(book.value())) {
        std::cout << "Updated successfully" << std::endl;
    } else {
        std::cout << "Book not found" << std::endl;
    }
}
```

### Delete

```cpp
bool deleteBook(int id)
```
- Permanently removes book from database
- Returns `true` if deleted, `false` if not found
- **Warning:** This cannot be undone!

**Example:**
```cpp
if (db.deleteBook(5)) {
    std::cout << "Book deleted" << std::endl;
} else {
    std::cout << "Book not found" << std::endl;
}
```

### Transactions

```cpp
void beginTransaction()
void commitTransaction()
void rollbackTransaction()
```
- Group multiple operations
- All succeed or all fail
- Use for data consistency

**Example:**
```cpp
try {
    db.beginTransaction();
    
    Book b1("Book 1", "Author 1", "", 100);
    Book b2("Book 2", "Author 2", "", 200);
    
    db.addBook(b1);
    db.addBook(b2);
    
    db.commitTransaction();  // Save both books
    std::cout << "Transaction successful" << std::endl;
    
} catch (const std::exception& e) {
    db.rollbackTransaction();  // Undo everything
    std::cerr << "Transaction failed: " << e.what() << std::endl;
}
```

### Utility

```cpp
void close()
```
- Manually close database connection
- Called automatically by destructor
- Safe to call multiple times

```cpp
bool isOpen() const
```
- Check if database is connected
- Returns `true` if open, `false` otherwise

## Error Handling

All methods throw `std::runtime_error` on database errors:

```cpp
try {
    Database db("my_books.db");
    db.addBook(book);
} catch (const std::runtime_error& e) {
    std::cerr << "Database error: " << e.what() << std::endl;
}
```

## Common Patterns

### Check Before Update/Delete

```cpp
auto book = db.getBook(id);
if (book.has_value()) {
    // Safe to update or delete
    db.updateBook(book.value());
}
```

### Search and Update

```cpp
auto books = db.searchBooks("tolkien");
for (auto& book : books) {
    book.setCurrentPage(0);  // Reset progress
    db.updateBook(book);
}
```

### Batch Operations with Transaction

```cpp
db.beginTransaction();
try {
    for (const auto& book : booksToAdd) {
        db.addBook(book);
    }
    db.commitTransaction();
} catch (...) {
    db.rollbackTransaction();
    throw;
}
```

### Safe Delete with Confirmation

```cpp
auto book = db.getBook(id);
if (book.has_value()) {
    std::cout << "Delete '" << book->getTitle() << "'? (y/n): ";
    char confirm;
    std::cin >> confirm;
    if (confirm == 'y' || confirm == 'Y') {
        db.deleteBook(id);
    }
}
```

## Tips & Best Practices

### ✅ DO

- Always check if `getBook()` returned a value before using it
- Use transactions for related operations
- Handle exceptions from database operations
- Close database connections when done (or use RAII)
- Use prepared statements (handled internally by Database class)

### ❌ DON'T

- Don't assume an ID exists before checking
- Don't ignore return values from `updateBook()` and `deleteBook()`
- Don't perform many individual operations when you could use a transaction
- Don't store sensitive data unencrypted
- Don't forget to set book IDs before calling `updateBook()`

## Performance Tips

1. **Use Transactions for Bulk Operations**
   ```cpp
   db.beginTransaction();
   for (int i = 0; i < 1000; i++) {
       db.addBook(books[i]);
   }
   db.commitTransaction();  // Much faster than 1000 individual commits!
   ```

2. **Search Performance**
   - Search by exact ID is fastest: `getBook(id)`
   - Text search is slower but still efficient for thousands of books
   - Future: Add indexes for better performance on large datasets

3. **Batch Reads**
   - `getAllBooks()` is efficient - fetches all at once
   - Better than calling `getBook()` in a loop

## Database File Location

The database file is created at the path you specify:

```cpp
// Relative path (in project directory)
Database db("books.db");

// Absolute path
Database db("/Users/name/Documents/books.db");

// Qt data directory (recommended for production)
QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
QString dbPath = dataDir + "/books.db";
Database db(dbPath.toStdString());
```

## Schema Information

Current database schema:

| Column | Type | Constraints | Description |
|--------|------|-------------|-------------|
| id | INTEGER | PRIMARY KEY AUTOINCREMENT | Unique book ID |
| title | TEXT | NOT NULL | Book title |
| author | TEXT | NOT NULL | Author name |
| isbn | TEXT | - | ISBN number (optional) |
| page_count | INTEGER | DEFAULT 0 | Total pages |
| current_page | INTEGER | DEFAULT 0 | Reading progress |
| start_date | INTEGER | - | Unix timestamp (optional) |
| completion_date | INTEGER | - | Unix timestamp (optional) |

## Troubleshooting

### "Failed to open database"
- Check file path is valid
- Ensure directory exists
- Check file permissions

### "Failed to insert book"
- Check book has title and author set
- Verify page count is non-negative
- Check ISBN length (must be 13 digits if provided)

### "Cannot update book: invalid ID"
- Book ID must be > 0
- Retrieve book from database first, then modify

### Book not found after adding
- Check return value from `addBook()`
- Verify database file location
- Check for exceptions during insert

## Example: Complete Book Lifecycle

```cpp
#include "core/database.h"
#include "core/book.h"
#include <iostream>

int main() {
    try {
        // 1. Open database
        Database db("my_library.db");
        
        // 2. Create and add a new book
        Book book("The Hobbit", "J.R.R. Tolkien", "9780547928227", 310);
        int id = db.addBook(book);
        std::cout << "Added book with ID: " << id << std::endl;
        
        // 3. Start reading
        auto myBook = db.getBook(id);
        if (myBook.has_value()) {
            myBook->setCurrentPage(50);
            db.updateBook(myBook.value());
            std::cout << "Progress: " << myBook->getProgressPercentage() << "%" << std::endl;
        }
        
        // 4. Continue reading
        myBook = db.getBook(id);  // Refresh from database
        if (myBook.has_value()) {
            myBook->setCurrentPage(150);
            db.updateBook(myBook.value());
            std::cout << "Progress: " << myBook->getProgressPercentage() << "%" << std::endl;
        }
        
        // 5. Finish the book
        myBook = db.getBook(id);
        if (myBook.has_value()) {
            myBook->markAsCompleted();
            db.updateBook(myBook.value());
            std::cout << "Book completed!" << std::endl;
        }
        
        // 6. View all books
        auto allBooks = db.getAllBooks();
        std::cout << "\nMy Library (" << allBooks.size() << " books):" << std::endl;
        for (const auto& b : allBooks) {
            std::cout << "  - " << b.getTitle();
            if (b.isCompleted()) {
                std::cout << " ✓";
            }
            std::cout << std::endl;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
```

## Next Steps

- Read `DATABASE_TUTORIAL_SUMMARY.md` for detailed learning material
- Check `src/main.cpp` for comprehensive examples
- Review `src/core/database.cpp` for implementation details
- Experiment with the test program to understand each operation

