/**
* @file book.h
* @brief Book entity class for the Personal Reading Management System
*
* This class represents a book in the user's collection with all
* relevant metadata and reading progress information.
*
* @author Shahzaib Ahmed
* @date October 12, 2025
*/

#ifndef BOOK_H
#define BOOK_H

#include <string> // included because we need to store the book title, author, publisher, year, genre, page count, word count
#include <chrono> // included because we need to store the date of acquisition, date of start reading, date of finish reading
#include <optional> // included because we need to store the optional fields like rating, review, etc.

/**
 * @brief Represents a book in the user's personal collection
 *
 * This class stores all information about a book including metadata
 * (title, author, ISBN) and reading progress (current page, start date).
 * It provides methods to access and modify this information safely.
 */

 class Book {
    public: 
        // ==== CONSTRUCTORS ====

        /**
         * @brief Default constructor - creates an emtpy book
         * 
         * Used when you need a book object but don't have the data yet.
         * all fields will be empty or set to default values.
         */
        Book();

        /**
         * @brief Parameterized constructor - creates a book wth the the initial data
         * 
         * @param title The title of the book
         * @param author The author of the book
         * @param isbn the ISBN number (optional)
         * @param pageCount Total number of pages in the book
         * 
         * This is the most common way to create a book when you have
         * the basic information available.
        */
        Book (const std::string& title, const std::string& author,
              const std::string& isbn ==, int pageCount = 0):
        
        // ==== GETTERS METHODS ====

        /**
         * @brief Get the title of the book
         * @return The title of the book
        */
        const std::string& getTitle() const;

        /**
         * @brief Get the author of the book
         * @return The book's author
         */
        const std::string& getAuthor() const;

        /**
         * @brief Get the ISBN of the book
         * @return The book's ISBN
         */
        const std::string& getISBN() const;

        /**
         * @brief Get the page count of the book
         * @return The book's page count
         */
        int getPageCount() const;

        /**
         * @brief Get the current page that the user is on
         * @return Current page that the user is on
         */
        int getCurrentPage() const;

         /**
          * @brief Get the date when reading was started
          * @return Completion date (empty if not started)
          */
        const std::optional<std::chrono::system_clock::time_point>& getStartDate() const;

          /**
           * @brief Get the date when reading was completed
           * @return Completion date (empty if not completed)
           */
        const std::optional<std::chrono::system_clock::time_point>& getCompletionDate() const;

          /**
           * @brief Calculate reading progress as a percentage
           * @return Progress percentage (0.0 to 100.0)
           * 
           * If page count is 0, returns 0.0
           * If current page is 0, returns 0.0
           * Otherwise retunrs (currentPage / pageCount) * 100.0
           */
        double getProgressPercentage() const; 

          // ==== SETTER METHODS ====

          /**
           * @brief Set the unique identifier for this book
           * @param id The unique ID (usually set by the database) 
           */
        void setId(int id); 

           /**
            * @brief Set the title of the book
            * @param title The new title
            */
        void setTitle(const std::string& title);

        /**
         * @brief Set the author of the book
         * @param author The new author
         */
        void setAuthor(const std::string& author);

        /**
         * @brief Set the ISBN of the book
         * @param isbn The new ISBN
         */
        void setISBN(const std::string& isbn);

        /**
         * @brief Set the page count of the book
         * @param pageCount The new page count
         */
        void setPageCount(int pageCount); 

        /**
         * @brief Set the current page that the user is on
         * @param currentPage The new current page
         */
        void setCurrentPage(int currentPage);

        /**
         * @brief Set the date when the reading was started
         * @param currentPage The new current page
         */
        void setStartDate(const std::chrono::system_clock::time_point& startDate);

        /**
         * @brief Set the date when reading was completed
         * @param completionDate The new completion date
         */
        void setCompletionDate(const std::chrono::system_clock::time_point& completionDate);

        // ==== UTILITY METHODS ====
        
        /**
         * @brief Check if the book has been started
         * @return True if current page > 0 or start date is set
         */
        bool isStarted() const;

        /**
         * @brief Check if the book has been completed
         * @return True if current page equals page count or completion date is set
         */
        bool isCompleted() const;

        /**
         * @brief Mark the book as completed
         *
         * Sets the current page to page count and setes completion date to now.
         */
        void markAsCompleted(); 

        /**
         * @brief Reset reading progress
         * 
         * Sets current page to 0 and clears start/completion dates.
         */
        void resetProgress();

    private:
        // ==== MEMBER VARIABLES ====

        int m_id; // unique identifier for the book
        std::string m_title; // title of the book
        std::string m_author; // author of the book
        std:: string m_isbn; // ISBN number of the book
        int m_pageCount; // total number of pages in the book
        int m_currentPage; // current page that the user is on
        std::optional<std::chrono::system_clock::time_point> m_startDate; // date when the reading was started
        std::optional<std::chrono::system_clock::time_point> m_completionDate; // date when the reading was completed
};

#endif // BOOK_H