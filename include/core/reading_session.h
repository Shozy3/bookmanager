/**
 * @file reading_session.h
 * @brief Reading Session entity class for the Personal Reading Management System
 * 
 * This class represents an individual reading session - a period of time spent
 * reading a specific book. It tracks duration, pages read, and allows calculation
 * of reading speed metrics.
 * 
 * @author Shahzaib Ahmed
 * @date October 16, 2025
 */

#ifndef READING_SESSION_H
#define READING_SESSION_H

#include <string>
#include <chrono>

/**
 * @brief Represents a single reading session for a book
 * 
 * A reading session captures:
 * - When the reading occurred
 * - How long it lasted
 * - How many pages were read
 * - Optional notes about the session
 * 
 * This enables tracking reading habits, calculating reading speed,
 * and providing insights about reading patterns.
 */
class ReadingSession {
public:
    // ==== CONSTRUCTORS ====
    
    /**
     * @brief Default constructor - creates an empty session
     */
    ReadingSession();
    
    /**
     * @brief Parameterized constructor - creates a session with data
     * 
     * @param bookId ID of the book being read
     * @param sessionDate When the session occurred
     * @param durationMinutes How long the session lasted (in minutes)
     * @param pagesRead Number of pages read during session
     * @param startPage Page number at start of session
     * @param endPage Page number at end of session
     * @param notes Optional notes about the session
     */
    ReadingSession(int bookId, 
                   const std::chrono::system_clock::time_point& sessionDate,
                   int durationMinutes,
                   int pagesRead,
                   int startPage,
                   int endPage,
                   const std::string& notes = "");
    
    // ==== GETTERS ====
    
    /**
     * @brief Get the session ID
     * @return Session ID (0 if not saved to database yet)
     */
    int getId() const;
    
    /**
     * @brief Get the book ID
     * @return ID of the book for this session
     */
    int getBookId() const;
    
    /**
     * @brief Get the session date/time
     * @return When the session occurred
     */
    const std::chrono::system_clock::time_point& getSessionDate() const;
    
    /**
     * @brief Get the duration in minutes
     * @return How long the session lasted
     */
    int getDurationMinutes() const;
    
    /**
     * @brief Get pages read
     * @return Number of pages read during session
     */
    int getPagesRead() const;
    
    /**
     * @brief Get starting page
     * @return Page number at start of session
     */
    int getStartPage() const;
    
    /**
     * @brief Get ending page
     * @return Page number at end of session
     */
    int getEndPage() const;
    
    /**
     * @brief Get session notes
     * @return Notes/comments about the session
     */
    const std::string& getNotes() const;
    
    // ==== SETTERS ====
    
    /**
     * @brief Set the session ID
     * @param id The new ID
     */
    void setId(int id);
    
    /**
     * @brief Set the book ID
     * @param bookId The book ID
     */
    void setBookId(int bookId);
    
    /**
     * @brief Set the session date/time
     * @param sessionDate When the session occurred
     */
    void setSessionDate(const std::chrono::system_clock::time_point& sessionDate);
    
    /**
     * @brief Set the duration
     * @param durationMinutes Duration in minutes
     */
    void setDurationMinutes(int durationMinutes);
    
    /**
     * @brief Set pages read
     * @param pagesRead Number of pages
     */
    void setPagesRead(int pagesRead);
    
    /**
     * @brief Set starting page
     * @param startPage Page number
     */
    void setStartPage(int startPage);
    
    /**
     * @brief Set ending page
     * @param endPage Page number
     */
    void setEndPage(int endPage);
    
    /**
     * @brief Set session notes
     * @param notes Notes/comments
     */
    void setNotes(const std::string& notes);
    
    // ==== UTILITY METHODS ====
    
    /**
     * @brief Calculate reading speed in pages per hour
     * @return Pages per hour (0.0 if duration is 0)
     */
    double getPagesPerHour() const;
    
    /**
     * @brief Calculate reading speed in pages per minute
     * @return Pages per minute (0.0 if duration is 0)
     */
    double getPagesPerMinute() const;
    
    /**
     * @brief Get duration as a formatted string (e.g., "1h 30m")
     * @return Human-readable duration
     */
    std::string getFormattedDuration() const;
    
    /**
     * @brief Check if this is a valid session
     * @return True if session has valid data
     */
    bool isValid() const;

private:
    // ==== MEMBER VARIABLES ====
    
    int m_id;                                               ///< Unique session ID
    int m_bookId;                                           ///< ID of the book
    std::chrono::system_clock::time_point m_sessionDate;    ///< When session occurred
    int m_durationMinutes;                                  ///< Duration in minutes
    int m_pagesRead;                                        ///< Pages read
    int m_startPage;                                        ///< Starting page
    int m_endPage;                                          ///< Ending page
    std::string m_notes;                                    ///< Session notes
};

#endif // READING_SESSION_H

