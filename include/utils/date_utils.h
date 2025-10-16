/**
 * @file date_utils.h
 * @brief Date and time utility functions for the Personal Reading Management System
 * 
 * Provides helper functions for formatting dates, calculating durations,
 * and converting between different time representations.
 * 
 * @author Shahzaib Ahmed
 * @date October 16, 2025
 */

#ifndef DATE_UTILS_H
#define DATE_UTILS_H

#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>

namespace DateUtils {

/**
 * @brief Format a time_point as a date string (e.g., "Oct 16, 2025")
 * 
 * @param timePoint The time point to format
 * @return Formatted date string
 */
inline std::string formatDate(const std::chrono::system_clock::time_point& timePoint) {
    std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
    std::tm* tm = std::localtime(&time);
    
    std::ostringstream oss;
    oss << std::put_time(tm, "%b %d, %Y");
    return oss.str();
}

/**
 * @brief Format a time_point as a date and time string (e.g., "Oct 16, 2025 2:30 PM")
 * 
 * @param timePoint The time point to format
 * @return Formatted date and time string
 */
inline std::string formatDateTime(const std::chrono::system_clock::time_point& timePoint) {
    std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
    std::tm* tm = std::localtime(&time);
    
    std::ostringstream oss;
    oss << std::put_time(tm, "%b %d, %Y %I:%M %p");
    return oss.str();
}

/**
 * @brief Format a time_point as a short date (e.g., "10/16/2025")
 * 
 * @param timePoint The time point to format
 * @return Formatted short date string
 */
inline std::string formatShortDate(const std::chrono::system_clock::time_point& timePoint) {
    std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
    std::tm* tm = std::localtime(&time);
    
    std::ostringstream oss;
    oss << std::put_time(tm, "%m/%d/%Y");
    return oss.str();
}

/**
 * @brief Format a time_point as a relative time (e.g., "2 days ago", "in 3 hours")
 * 
 * @param timePoint The time point to format
 * @return Relative time string
 */
inline std::string formatRelativeTime(const std::chrono::system_clock::time_point& timePoint) {
    auto now = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - timePoint);
    
    int64_t seconds = duration.count();
    
    if (seconds < 0) {
        // Future time
        seconds = -seconds;
        if (seconds < 60) return "in a moment";
        if (seconds < 3600) return "in " + std::to_string(seconds / 60) + " minutes";
        if (seconds < 86400) return "in " + std::to_string(seconds / 3600) + " hours";
        return "in " + std::to_string(seconds / 86400) + " days";
    } else {
        // Past time
        if (seconds < 60) return "just now";
        if (seconds < 3600) return std::to_string(seconds / 60) + " minutes ago";
        if (seconds < 86400) return std::to_string(seconds / 3600) + " hours ago";
        if (seconds < 604800) return std::to_string(seconds / 86400) + " days ago";
        if (seconds < 2592000) return std::to_string(seconds / 604800) + " weeks ago";
        if (seconds < 31536000) return std::to_string(seconds / 2592000) + " months ago";
        return std::to_string(seconds / 31536000) + " years ago";
    }
}

/**
 * @brief Calculate duration between two time points as human-readable string
 * 
 * @param start Start time
 * @param end End time
 * @return Duration string (e.g., "2h 30m", "5 days")
 */
inline std::string formatDuration(const std::chrono::system_clock::time_point& start,
                                  const std::chrono::system_clock::time_point& end) {
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    int64_t totalSeconds = duration.count();
    
    if (totalSeconds < 0) {
        totalSeconds = -totalSeconds;
    }
    
    int days = totalSeconds / 86400;
    int hours = (totalSeconds % 86400) / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    
    std::ostringstream oss;
    
    if (days > 0) {
        oss << days << (days == 1 ? " day" : " days");
        if (hours > 0) {
            oss << " " << hours << "h";
        }
    } else if (hours > 0) {
        oss << hours << "h";
        if (minutes > 0) {
            oss << " " << minutes << "m";
        }
    } else {
        oss << minutes << "m";
    }
    
    return oss.str();
}

/**
 * @brief Get current date/time
 * 
 * @return Current time point
 */
inline std::chrono::system_clock::time_point now() {
    return std::chrono::system_clock::now();
}

/**
 * @brief Get start of today (midnight)
 * 
 * @return Time point representing start of current day
 */
inline std::chrono::system_clock::time_point startOfToday() {
    std::time_t now_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm* now_tm = std::localtime(&now_time);
    now_tm->tm_hour = 0;
    now_tm->tm_min = 0;
    now_tm->tm_sec = 0;
    return std::chrono::system_clock::from_time_t(std::mktime(now_tm));
}

/**
 * @brief Check if a date is today
 * 
 * @param timePoint The time point to check
 * @return True if the date is today
 */
inline bool isToday(const std::chrono::system_clock::time_point& timePoint) {
    auto today = startOfToday();
    auto tomorrow = today + std::chrono::hours(24);
    return timePoint >= today && timePoint < tomorrow;
}

/**
 * @brief Calculate days between two dates
 * 
 * @param start Start date
 * @param end End date
 * @return Number of days between dates
 */
inline int daysBetween(const std::chrono::system_clock::time_point& start,
                       const std::chrono::system_clock::time_point& end) {
    auto duration = std::chrono::duration_cast<std::chrono::hours>(end - start);
    return duration.count() / 24;
}

} // namespace DateUtils

#endif // DATE_UTILS_H

