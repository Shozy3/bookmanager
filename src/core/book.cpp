/**
 * @file book.cpp
 * @brief Implementation of the Book class for the Personal Reading Management System (PRMS)
 * 
 * This file contains the implementation of the methods declared
 * in the Book class header file.
 * @author Shahzaib Ahmed
 * @date October 12, 2025
 */

#include "book.h"
#include <stdexcept>
#include <algorithm>

// ==== CONSTRUCTORS ====

/**
 * @brief Default constructor - creates an empty book
 * 
 * Initializes all member variables to their default values.
 * This is used when you need a Book object but don't have
 * the data yet (like when loading from a database).
 */
Book::Book()
    : m_id(0) // Initialize ID to 0 (not set)
    , m_title("") // Initialize title to empty string
    , m_author("") // Initialize author to empty string
    , m_isbn("") // Initialize ISBN to empty string
    , m_pageCount(0) // Initialize page count to 0
    , m_currentPage(0) // Initialize current page to 0
    , m_startDate(std::nullopt) // Initialize start date to "not set"
    , m_completionDate(std::nullopt) // Initialize completion date to "not set"
    , m_genre("") // Initialize genre to empty
    , m_publisher("") // Initialize publisher to empty
    , m_yearPublished(0) // Initialize year to 0 (not set)
    , m_notes("") // Initialize notes to empty
    , m_review("") // Initialize review to empty
    , m_rating(0) // Initialize rating to 0 (unrated)
    , m_coverPath("") // Initialize cover path to empty
    , m_dateAdded(std::chrono::system_clock::now()) // Initialize to current time
    , m_status(0) // Initialize to ToRead status
{
    // Constructor body is empty because we initialized everything above
    // This is called "member initializer list"
}

/**
 * @brief Parameterized constructor - creates a book with the initial data
 * 
 * @param title The title of the book
 * @param author The author of the book
 * @param isbn The ISBN number of the book (optional, defaults to empty string)
 * @param pageCount The page count of the book (optional, defaults to 0)
 */
Book::Book(const std::string& title, const std::string& author,
           const std::string& isbn, int pageCount)
    : m_id(0) // ID starts at 0 (will be set by the database)
    , m_title(title) // Initialize title to the passed value
    , m_author(author) // Initialize author to the passed value
    , m_isbn(isbn) // Initialize ISBN to the passed value
    , m_pageCount(pageCount) // Initialize page count to the passed value
    , m_currentPage(0) // Start at page 0 (not started yet)
    , m_startDate(std::nullopt) // Initialize start date to "not set"
    , m_completionDate(std::nullopt) // Initialize completion date to "not set"
    , m_genre("") // Initialize genre to empty
    , m_publisher("") // Initialize publisher to empty
    , m_yearPublished(0) // Initialize year to 0 (not set)
    , m_notes("") // Initialize notes to empty
    , m_review("") // Initialize review to empty
    , m_rating(0) // Initialize rating to 0 (unrated)
    , m_coverPath("") // Initialize cover path to empty
    , m_dateAdded(std::chrono::system_clock::now()) // Initialize to current time
    , m_status(0) // Initialize to ToRead status
{
    // Validate the input data

    if (pageCount < 0) {
        throw std::invalid_argument("Page count cannot be negative");
    }

    if (title.empty()) {
        throw std::invalid_argument("Title cannot be empty");
    }

    if (author.empty()) {
        throw std::invalid_argument("Author cannot be empty");
    }

    if (!isbn.empty() && isbn.length() != 13) {
        throw std::invalid_argument("ISBN must be 13 digits");
    }
}

// ==== GETTER METHODS ====

/**
 * @brief Get the unique ID of the book
 * @return The book's ID (0 if not set)
 */
int Book::getId() const {
    return m_id;
}

/**
 * @brief Get the title of the book
 * @return The title of the book
 */
const std::string& Book::getTitle() const {
    return m_title;
}

/**
 * @brief Get the author of the book
 * @return The book's author
 */
const std::string& Book::getAuthor() const {
    return m_author;
}

/**
 * @brief Get the ISBN of the book
 * @return The book's ISBN
 */
const std::string& Book::getISBN() const {
    return m_isbn;
}

/**
 * @brief Get the page count of the book
 * @return The book's page count
 */
int Book::getPageCount() const {
    return m_pageCount;
}

/**
 * @brief Get the current page that the user is on
 * @return Current page that the user is on
 */
int Book::getCurrentPage() const {
    return m_currentPage;
}

/**
 * @brief Get the date when reading was started
 */
const std::optional<std::chrono::system_clock::time_point>& Book::getStartDate() const {
    return m_startDate;
}

/**
 * @brief Get the date when reading was completed
 */
const std::optional<std::chrono::system_clock::time_point>& Book::getCompletionDate() const {
    return m_completionDate;
}

/**
 * @brief Calculate reading progress as a percentage
 * @return Progress percentage (0.0 to 100.0)
 * 
 * If page count is 0, returns 0.0
 * If current page is 0, returns 0.0
 * Otherwise retunrs (currentPage / pageCount) * 100.0
 */
double Book::getProgressPercentage() const {
    // Handle edge cases
    if (m_pageCount <= 0) {
        return 0.0; // No progress if page count is 0
    }

    if (m_currentPage <= 0) {
        return 0.0; // No progress if current page is 0
    }

    // Calculate percentage: (current page / total number of pages) * 100.0
    double percentage = (static_cast<double>(m_currentPage) / m_pageCount) * 100.0;

    // Cap at 100% (in case current page > page count) 
    // TODO: Add a warning if current page > page count so this isn't neccesary
    return std::min(percentage, 100.0);
}

// ==== SETTER METHODS ====

/**
 * @brief Set the unique identifier for this book
 * @param id The unique ID (usually set by the database)
 */
void Book::setId(int id) {
    if (id < 0) {
        throw std::invalid_argument("ID cannot be negative");
    }
    m_id = id;
}

/**
 * @brief Set the title of the book
 * @param title The new title
 */
void Book::setTitle(const std::string& title) {
    if (title.empty()) {
        throw std::invalid_argument("Title cannot be empty");
    }
    m_title = title;
}

/**
 * @brief Set the author of the book
 * @param author The new author
 */
void Book::setAuthor(const std::string& author) {
    if (author.empty()) {
        throw std::invalid_argument("Author cannot be empty");
    }
    m_author = author;
}

/**
 * @brief Set the ISBN of the book
 * @param isbn The new ISBN
 */
void Book::setISBN(const std::string& isbn) {
    if (!isbn.empty() && isbn.length() != 13) {
        throw std::invalid_argument("ISBN must be 13 digits");
    }
    m_isbn = isbn;
}

/**
 * @brief Set the page count of the book
 * @param pageCount The new page count
 */
void Book::setPageCount(int pageCount) {
    if (pageCount < 0) {
        throw std::invalid_argument("Page count cannot be negative");
    }
    m_pageCount = pageCount;

    // If current page is greater than page count, set it to page count
    if (m_currentPage > pageCount) {
        m_currentPage = pageCount;
    }
}

/**
 * @brief Set the current page that the user is on
 * @param currentPage The new current page
 */
void Book::setCurrentPage(int currentPage) {
    if (currentPage < 0) {
        throw std::invalid_argument("Current page cannot be negative");
    }

    if (currentPage > m_pageCount && m_pageCount > 0) {
        throw std::invalid_argument("Current page cannot be greater than page count");
    }

    m_currentPage = currentPage;

    // Automatically set start date if not set and we're starting to read
    if (currentPage > 0 && !m_startDate) {
        m_startDate = std::chrono::system_clock::now();
    }

    // Automatically set completion date if we've reached the end
    if (currentPage == m_pageCount && m_pageCount > 0 && !m_completionDate.has_value()) {
        m_completionDate = std::chrono::system_clock::now();
    }
}

/**
 * @brief Set the date when the reading was started
 * @param startDate The new start date
 */
void Book::setStartDate(const std::chrono::system_clock::time_point& startDate) {
    m_startDate = startDate;
}

/**
 * @brief Set the date when the reading was completed
 * @param completionDate The new completion date
 */
void Book::setCompletionDate(const std::chrono::system_clock::time_point& completionDate) {
    m_completionDate = completionDate;
}


// ==== ADDITIONAL GETTERS ====

/**
 * @brief Get the genre of the book
 */
const std::string& Book::getGenre() const {
    return m_genre;
}

/**
 * @brief Get the publisher of the book
 */
const std::string& Book::getPublisher() const {
    return m_publisher;
}

/**
 * @brief Get the year published
 */
int Book::getYearPublished() const {
    return m_yearPublished;
}

/**
 * @brief Get personal notes
 */
const std::string& Book::getNotes() const {
    return m_notes;
}

/**
 * @brief Get personal review
 */
const std::string& Book::getReview() const {
    return m_review;
}

/**
 * @brief Get the rating
 */
int Book::getRating() const {
    return m_rating;
}

/**
 * @brief Get the cover image path
 */
const std::string& Book::getCoverPath() const {
    return m_coverPath;
}

/**
 * @brief Get the date when book was added
 */
const std::chrono::system_clock::time_point& Book::getDateAdded() const {
    return m_dateAdded;
}

/**
 * @brief Get the reading status
 */
int Book::getStatus() const {
    return m_status;
}

// ==== ADDITIONAL SETTERS ====

/**
 * @brief Set the genre
 */
void Book::setGenre(const std::string& genre) {
    m_genre = genre;
}

/**
 * @brief Set the publisher
 */
void Book::setPublisher(const std::string& publisher) {
    m_publisher = publisher;
}

/**
 * @brief Set the year published
 */
void Book::setYearPublished(int year) {
    if (year < 0 || year > 9999) {
        throw std::invalid_argument("Invalid year");
    }
    m_yearPublished = year;
}

/**
 * @brief Set personal notes
 */
void Book::setNotes(const std::string& notes) {
    m_notes = notes;
}

/**
 * @brief Set personal review
 */
void Book::setReview(const std::string& review) {
    m_review = review;
}

/**
 * @brief Set the rating
 */
void Book::setRating(int rating) {
    if (rating < 0 || rating > 5) {
        throw std::invalid_argument("Rating must be between 0 and 5");
    }
    m_rating = rating;
}

/**
 * @brief Set the cover image path
 */
void Book::setCoverPath(const std::string& path) {
    m_coverPath = path;
}

/**
 * @brief Set the date added
 */
void Book::setDateAdded(const std::chrono::system_clock::time_point& date) {
    m_dateAdded = date;
}

/**
 * @brief Set the reading status
 */
void Book::setStatus(int status) {
    if (status < 0 || status > 4) {
        throw std::invalid_argument("Invalid status code");
    }
    m_status = status;
}

// ==== UTILITY METHODS ====

/**
 * @brief Check if the book has been started
 * @return True if currrent page is greater than 0 or start date is set
 */
bool Book::isStarted() const {
    return m_currentPage > 0 || m_startDate.has_value();
}

/**
 * @brief Check if the book has been completed
 * @return True if current page equals page count or the completion date is set
 */
bool Book::isCompleted() const {
    return (m_pageCount > 0 && m_currentPage == m_pageCount) || m_completionDate.has_value();
}

/**
 * @brief Mark the book as completed
 */
void Book::markAsCompleted() {
    if (m_pageCount <= 0) {
        throw std::runtime_error("Cannot mark book as completed: page count is 0");
    }

    m_currentPage = m_pageCount;
    m_completionDate = std::chrono::system_clock::now();
}

/**
 * @brief Reset the reading progress
 */
void Book::resetProgress() {
    m_currentPage = 0;
    m_startDate = std::nullopt;
    m_completionDate = std::nullopt;
}