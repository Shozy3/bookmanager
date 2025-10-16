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
              const std::string& isbn = "", int pageCount = 0);
        
        // ==== GETTERS METHODS ====

        /**
         * @brief Get the unique ID of the book
         * @return The book's ID (0 if not set)
         */
        int getId() const;

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

        // ==== ADDITIONAL GETTERS ====

        /**
         * @brief Get the genre of the book
         * @return The book's genre
         */
        const std::string& getGenre() const;

        /**
         * @brief Get the publisher of the book
         * @return The book's publisher
         */
        const std::string& getPublisher() const;

        /**
         * @brief Get the year published
         * @return Year of publication
         */
        int getYearPublished() const;

        /**
         * @brief Get personal notes
         * @return The notes
         */
        const std::string& getNotes() const;

        /**
         * @brief Get personal review
         * @return The review
         */
        const std::string& getReview() const;

        /**
         * @brief Get the rating
         * @return Rating (0-5 stars, 0 = unrated)
         */
        int getRating() const;

        /**
         * @brief Get the cover image path
         * @return Path to cover image
         */
        const std::string& getCoverPath() const;

        /**
         * @brief Get the date when book was added
         * @return Date added to library
         */
        const std::chrono::system_clock::time_point& getDateAdded() const;

        /**
         * @brief Get the reading status
         * @return Status (0=ToRead, 1=Reading, 2=Completed, 3=DNF, 4=Wishlist)
         */
        int getStatus() const;

        // ==== ADDITIONAL SETTERS ====

        /**
         * @brief Set the genre
         * @param genre The new genre
         */
        void setGenre(const std::string& genre);

        /**
         * @brief Set the publisher
         * @param publisher The new publisher
         */
        void setPublisher(const std::string& publisher);

        /**
         * @brief Set the year published
         * @param year Year of publication
         */
        void setYearPublished(int year);

        /**
         * @brief Set personal notes
         * @param notes The notes
         */
        void setNotes(const std::string& notes);

        /**
         * @brief Set personal review
         * @param review The review
         */
        void setReview(const std::string& review);

        /**
         * @brief Set the rating
         * @param rating Rating (0-5 stars)
         */
        void setRating(int rating);

        /**
         * @brief Set the cover image path
         * @param path Path to cover image
         */
        void setCoverPath(const std::string& path);

        /**
         * @brief Set the date added
         * @param date Date when added
         */
        void setDateAdded(const std::chrono::system_clock::time_point& date);

        /**
         * @brief Set the reading status
         * @param status Status code
         */
        void setStatus(int status);

    private:
        // ==== MEMBER VARIABLES ====

        int m_id; // unique identifier for the book
        std::string m_title; // title of the book
        std::string m_author; // author of the book
        std::string m_isbn; // ISBN number of the book
        int m_pageCount; // total number of pages in the book
        int m_currentPage; // current page that the user is on
        std::optional<std::chrono::system_clock::time_point> m_startDate; // date when the reading was started
        std::optional<std::chrono::system_clock::time_point> m_completionDate; // date when the reading was completed
        
        // Additional fields for enhanced functionality
        std::string m_genre; // genre/category of the book
        std::string m_publisher; // publisher name
        int m_yearPublished; // year of publication
        std::string m_notes; // personal notes about the book
        std::string m_review; // personal review
        int m_rating; // rating (0-5 stars, 0 = unrated)
        std::string m_coverPath; // path to cover image file
        std::chrono::system_clock::time_point m_dateAdded; // when book was added to library
        int m_status; // reading status (0=ToRead, 1=Reading, 2=Completed, 3=DNF, 4=Wishlist)
};

#endif // BOOK_H