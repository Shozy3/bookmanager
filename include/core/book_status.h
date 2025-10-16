/**
 * @file book_status.h
 * @brief Book status/category enumeration for the Personal Reading Management System
 * 
 * This file defines the possible states a book can be in, along with helper
 * functions for converting between status codes and human-readable strings.
 * 
 * @author Shahzaib Ahmed
 * @date October 16, 2025
 */

#ifndef BOOK_STATUS_H
#define BOOK_STATUS_H

#include <string>
#include <vector>

/**
 * @brief Enumeration of possible book states
 * 
 * These states represent the user's reading relationship with a book.
 * The values match what's stored in the database (Book.m_status field).
 */
enum class BookStatus {
    ToRead = 0,      ///< Book is in reading queue but not started
    Reading = 1,     ///< Currently being read
    Completed = 2,   ///< Finished reading
    DNF = 3,         ///< Did Not Finish - abandoned
    Wishlist = 4     ///< Want to acquire/read in future
};

/**
 * @brief Convert BookStatus enum to human-readable string
 * 
 * @param status The status to convert
 * @return String representation (e.g., "Reading", "Completed")
 */
inline std::string bookStatusToString(BookStatus status) {
    switch (status) {
        case BookStatus::ToRead:    return "To Read";
        case BookStatus::Reading:   return "Reading";
        case BookStatus::Completed: return "Completed";
        case BookStatus::DNF:       return "Did Not Finish";
        case BookStatus::Wishlist:  return "Wishlist";
        default:                     return "Unknown";
    }
}

/**
 * @brief Convert BookStatus enum to integer code
 * 
 * @param status The status to convert
 * @return Integer code for database storage
 */
inline int bookStatusToInt(BookStatus status) {
    return static_cast<int>(status);
}

/**
 * @brief Convert integer code to BookStatus enum
 * 
 * @param code The integer code from database
 * @return Corresponding BookStatus
 */
inline BookStatus intToBookStatus(int code) {
    if (code >= 0 && code <= 4) {
        return static_cast<BookStatus>(code);
    }
    return BookStatus::ToRead; // Default
}

/**
 * @brief Get all possible book statuses
 * 
 * @return Vector of all BookStatus values
 */
inline std::vector<BookStatus> getAllBookStatuses() {
    return {
        BookStatus::ToRead,
        BookStatus::Reading,
        BookStatus::Completed,
        BookStatus::DNF,
        BookStatus::Wishlist
    };
}

/**
 * @brief Get all status names
 * 
 * @return Vector of status names as strings
 */
inline std::vector<std::string> getAllBookStatusNames() {
    std::vector<std::string> names;
    for (const auto& status : getAllBookStatuses()) {
        names.push_back(bookStatusToString(status));
    }
    return names;
}

#endif // BOOK_STATUS_H

