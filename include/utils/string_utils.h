/**
 * @file string_utils.h
 * @brief String utility functions for the Personal Reading Management System
 * 
 * Provides helper functions for string manipulation, formatting, and validation.
 * 
 * @author Shahzaib Ahmed
 * @date October 16, 2025
 */

#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <vector>

namespace StringUtils {

/**
 * @brief Trim whitespace from the left side of a string
 * 
 * @param str String to trim
 * @return Trimmed string
 */
inline std::string ltrim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    return (start == std::string::npos) ? "" : str.substr(start);
}

/**
 * @brief Trim whitespace from the right side of a string
 * 
 * @param str String to trim
 * @return Trimmed string
 */
inline std::string rtrim(const std::string& str) {
    size_t end = str.find_last_not_of(" \t\n\r");
    return (end == std::string::npos) ? "" : str.substr(0, end + 1);
}

/**
 * @brief Trim whitespace from both sides of a string
 * 
 * @param str String to trim
 * @return Trimmed string
 */
inline std::string trim(const std::string& str) {
    return ltrim(rtrim(str));
}

/**
 * @brief Convert string to lowercase
 * 
 * @param str String to convert
 * @return Lowercase string
 */
inline std::string toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

/**
 * @brief Convert string to uppercase
 * 
 * @param str String to convert
 * @return Uppercase string
 */
inline std::string toUpper(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return result;
}

/**
 * @brief Truncate a string to a maximum length with ellipsis
 * 
 * @param str String to truncate
 * @param maxLength Maximum length (including ellipsis)
 * @param ellipsis Ellipsis string (default: "...")
 * @return Truncated string
 */
inline std::string truncate(const std::string& str, size_t maxLength, 
                           const std::string& ellipsis = "...") {
    if (str.length() <= maxLength) {
        return str;
    }
    if (maxLength <= ellipsis.length()) {
        return ellipsis.substr(0, maxLength);
    }
    return str.substr(0, maxLength - ellipsis.length()) + ellipsis;
}

/**
 * @brief Check if a string contains a substring (case-insensitive)
 * 
 * @param str String to search in
 * @param substr Substring to search for
 * @return True if substring is found
 */
inline bool containsIgnoreCase(const std::string& str, const std::string& substr) {
    std::string lowerStr = toLower(str);
    std::string lowerSubstr = toLower(substr);
    return lowerStr.find(lowerSubstr) != std::string::npos;
}

/**
 * @brief Check if a string starts with a prefix
 * 
 * @param str String to check
 * @param prefix Prefix to check for
 * @return True if string starts with prefix
 */
inline bool startsWith(const std::string& str, const std::string& prefix) {
    if (str.length() < prefix.length()) {
        return false;
    }
    return str.substr(0, prefix.length()) == prefix;
}

/**
 * @brief Check if a string ends with a suffix
 * 
 * @param str String to check
 * @param suffix Suffix to check for
 * @return True if string ends with suffix
 */
inline bool endsWith(const std::string& str, const std::string& suffix) {
    if (str.length() < suffix.length()) {
        return false;
    }
    return str.substr(str.length() - suffix.length()) == suffix;
}

/**
 * @brief Split a string by a delimiter
 * 
 * @param str String to split
 * @param delimiter Delimiter character
 * @return Vector of string parts
 */
inline std::vector<std::string> split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    
    return tokens;
}

/**
 * @brief Join a vector of strings with a delimiter
 * 
 * @param strings Vector of strings to join
 * @param delimiter Delimiter string
 * @return Joined string
 */
inline std::string join(const std::vector<std::string>& strings, 
                       const std::string& delimiter) {
    std::ostringstream oss;
    for (size_t i = 0; i < strings.size(); ++i) {
        if (i > 0) {
            oss << delimiter;
        }
        oss << strings[i];
    }
    return oss.str();
}

/**
 * @brief Replace all occurrences of a substring with another string
 * 
 * @param str String to modify
 * @param from Substring to replace
 * @param to Replacement string
 * @return Modified string
 */
inline std::string replaceAll(std::string str, const std::string& from, 
                             const std::string& to) {
    size_t pos = 0;
    while ((pos = str.find(from, pos)) != std::string::npos) {
        str.replace(pos, from.length(), to);
        pos += to.length();
    }
    return str;
}

/**
 * @brief Validate ISBN-13 format
 * 
 * @param isbn ISBN string to validate
 * @return True if valid ISBN-13 format
 */
inline bool isValidISBN13(const std::string& isbn) {
    // Remove hyphens and spaces
    std::string cleaned = isbn;
    cleaned.erase(std::remove_if(cleaned.begin(), cleaned.end(),
                                 [](char c) { return c == '-' || c == ' '; }),
                 cleaned.end());
    
    // Check length
    if (cleaned.length() != 13) {
        return false;
    }
    
    // Check all digits
    if (!std::all_of(cleaned.begin(), cleaned.end(), ::isdigit)) {
        return false;
    }
    
    // Validate checksum
    int sum = 0;
    for (size_t i = 0; i < 12; ++i) {
        int digit = cleaned[i] - '0';
        sum += (i % 2 == 0) ? digit : digit * 3;
    }
    int checkDigit = (10 - (sum % 10)) % 10;
    return checkDigit == (cleaned[12] - '0');
}

/**
 * @brief Format an ISBN with hyphens
 * 
 * @param isbn ISBN string (13 digits)
 * @return Formatted ISBN (e.g., 978-0-123456-78-9)
 */
inline std::string formatISBN(const std::string& isbn) {
    // Remove existing hyphens and spaces
    std::string cleaned = isbn;
    cleaned.erase(std::remove_if(cleaned.begin(), cleaned.end(),
                                 [](char c) { return c == '-' || c == ' '; }),
                 cleaned.end());
    
    if (cleaned.length() != 13) {
        return isbn; // Return original if not valid length
    }
    
    // Format as 978-0-123456-78-9 (simplified formatting)
    return cleaned.substr(0, 3) + "-" + cleaned.substr(3, 1) + "-" +
           cleaned.substr(4, 6) + "-" + cleaned.substr(10, 2) + "-" + cleaned.substr(12, 1);
}

/**
 * @brief Check if a string is empty or contains only whitespace
 * 
 * @param str String to check
 * @return True if string is blank
 */
inline bool isBlank(const std::string& str) {
    return trim(str).empty();
}

/**
 * @brief Capitalize first letter of each word
 * 
 * @param str String to capitalize
 * @return Capitalized string
 */
inline std::string toTitleCase(const std::string& str) {
    std::string result = str;
    bool capitalizeNext = true;
    
    for (char& c : result) {
        if (std::isspace(c)) {
            capitalizeNext = true;
        } else if (capitalizeNext) {
            c = std::toupper(c);
            capitalizeNext = false;
        } else {
            c = std::tolower(c);
        }
    }
    
    return result;
}

} // namespace StringUtils

#endif // STRING_UTILS_H

