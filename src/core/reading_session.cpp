/**
 * @file reading_session.cpp
 * @brief Implementation of the ReadingSession class
 * 
 * @author Shahzaib Ahmed
 * @date October 16, 2025
 */

#include "core/reading_session.h"
#include <stdexcept>
#include <sstream>
#include <iomanip>

// ==== CONSTRUCTORS ====

ReadingSession::ReadingSession()
    : m_id(0)
    , m_bookId(0)
    , m_sessionDate(std::chrono::system_clock::now())
    , m_durationMinutes(0)
    , m_pagesRead(0)
    , m_startPage(0)
    , m_endPage(0)
    , m_notes("")
{
}

ReadingSession::ReadingSession(int bookId,
                               const std::chrono::system_clock::time_point& sessionDate,
                               int durationMinutes,
                               int pagesRead,
                               int startPage,
                               int endPage,
                               const std::string& notes)
    : m_id(0)
    , m_bookId(bookId)
    , m_sessionDate(sessionDate)
    , m_durationMinutes(durationMinutes)
    , m_pagesRead(pagesRead)
    , m_startPage(startPage)
    , m_endPage(endPage)
    , m_notes(notes)
{
    // Validate input
    if (bookId <= 0) {
        throw std::invalid_argument("Book ID must be positive");
    }
    if (durationMinutes < 0) {
        throw std::invalid_argument("Duration cannot be negative");
    }
    if (pagesRead < 0) {
        throw std::invalid_argument("Pages read cannot be negative");
    }
    if (startPage < 0 || endPage < 0) {
        throw std::invalid_argument("Page numbers cannot be negative");
    }
    if (endPage < startPage) {
        throw std::invalid_argument("End page cannot be before start page");
    }
}

// ==== GETTERS ====

int ReadingSession::getId() const {
    return m_id;
}

int ReadingSession::getBookId() const {
    return m_bookId;
}

const std::chrono::system_clock::time_point& ReadingSession::getSessionDate() const {
    return m_sessionDate;
}

int ReadingSession::getDurationMinutes() const {
    return m_durationMinutes;
}

int ReadingSession::getPagesRead() const {
    return m_pagesRead;
}

int ReadingSession::getStartPage() const {
    return m_startPage;
}

int ReadingSession::getEndPage() const {
    return m_endPage;
}

const std::string& ReadingSession::getNotes() const {
    return m_notes;
}

// ==== SETTERS ====

void ReadingSession::setId(int id) {
    if (id < 0) {
        throw std::invalid_argument("ID cannot be negative");
    }
    m_id = id;
}

void ReadingSession::setBookId(int bookId) {
    if (bookId <= 0) {
        throw std::invalid_argument("Book ID must be positive");
    }
    m_bookId = bookId;
}

void ReadingSession::setSessionDate(const std::chrono::system_clock::time_point& sessionDate) {
    m_sessionDate = sessionDate;
}

void ReadingSession::setDurationMinutes(int durationMinutes) {
    if (durationMinutes < 0) {
        throw std::invalid_argument("Duration cannot be negative");
    }
    m_durationMinutes = durationMinutes;
}

void ReadingSession::setPagesRead(int pagesRead) {
    if (pagesRead < 0) {
        throw std::invalid_argument("Pages read cannot be negative");
    }
    m_pagesRead = pagesRead;
}

void ReadingSession::setStartPage(int startPage) {
    if (startPage < 0) {
        throw std::invalid_argument("Start page cannot be negative");
    }
    m_startPage = startPage;
}

void ReadingSession::setEndPage(int endPage) {
    if (endPage < 0) {
        throw std::invalid_argument("End page cannot be negative");
    }
    m_endPage = endPage;
}

void ReadingSession::setNotes(const std::string& notes) {
    m_notes = notes;
}

// ==== UTILITY METHODS ====

double ReadingSession::getPagesPerHour() const {
    if (m_durationMinutes == 0) {
        return 0.0;
    }
    double hours = m_durationMinutes / 60.0;
    return m_pagesRead / hours;
}

double ReadingSession::getPagesPerMinute() const {
    if (m_durationMinutes == 0) {
        return 0.0;
    }
    return static_cast<double>(m_pagesRead) / m_durationMinutes;
}

std::string ReadingSession::getFormattedDuration() const {
    int hours = m_durationMinutes / 60;
    int minutes = m_durationMinutes % 60;
    
    std::ostringstream oss;
    
    if (hours > 0) {
        oss << hours << "h";
        if (minutes > 0) {
            oss << " " << minutes << "m";
        }
    } else {
        oss << minutes << "m";
    }
    
    return oss.str();
}

bool ReadingSession::isValid() const {
    return m_bookId > 0 && m_durationMinutes >= 0 && m_pagesRead >= 0 &&
           m_startPage >= 0 && m_endPage >= m_startPage;
}

