/**
 * @file theme.h
 * @brief Theme manager for the Personal Reading Management System
 * 
 * Provides Material Design-inspired color palettes and stylesheet generation
 * for dark and light themes. Implements Singleton pattern for global access.
 * 
 * @author Shahzaib Ahmed
 * @date October 16, 2025
 */

#ifndef THEME_H
#define THEME_H

#include <QString>
#include <QColor>
#include <QApplication>

/**
 * @brief Theme modes available in the application
 */
enum class ThemeMode {
    Light,  ///< Light theme
    Dark    ///< Dark theme
};

/**
 * @brief Material Design-inspired theme manager (Singleton)
 * 
 * This class manages application theming, providing:
 * - Dark and Light theme support
 * - Material Design color palettes
 * - Dynamic stylesheet generation
 * - Theme switching without restart
 */
class Theme {
public:
    /**
     * @brief Get the singleton instance
     * @return Reference to the Theme instance
     */
    static Theme& instance();
    
    // Delete copy constructor and assignment operator
    Theme(const Theme&) = delete;
    Theme& operator=(const Theme&) = delete;
    
    /**
     * @brief Get current theme mode
     * @return Current ThemeMode
     */
    ThemeMode getCurrentMode() const;
    
    /**
     * @brief Set theme mode
     * @param mode Theme mode to set
     */
    void setThemeMode(ThemeMode mode);
    
    /**
     * @brief Toggle between light and dark themes
     */
    void toggleTheme();
    
    /**
     * @brief Apply current theme to the application
     */
    void applyTheme();
    
    /**
     * @brief Generate stylesheet for current theme
     * @return Qt stylesheet string
     */
    QString generateStylesheet() const;
    
    // ==== Color Accessors ====
    
    QColor getPrimaryColor() const;
    QColor getPrimaryDarkColor() const;
    QColor getPrimaryLightColor() const;
    QColor getAccentColor() const;
    QColor getBackgroundColor() const;
    QColor getSurfaceColor() const;
    QColor getCardColor() const;
    QColor getTextPrimaryColor() const;
    QColor getTextSecondaryColor() const;
    QColor getBorderColor() const;
    QColor getHoverColor() const;
    QColor getSelectedColor() const;
    QColor getErrorColor() const;
    QColor getSuccessColor() const;
    QColor getWarningColor() const;
    
private:
    /**
     * @brief Private constructor for Singleton
     */
    Theme();
    
    /**
     * @brief Load theme from settings
     */
    void loadThemeFromSettings();
    
    /**
     * @brief Save theme to settings
     */
    void saveThemeToSettings();
    
    ThemeMode m_currentMode;  ///< Current theme mode
    
    // ==== Material Design Color Palette ====
    
    // Light Theme Colors
    struct LightTheme {
        static constexpr const char* PRIMARY = "#1976D2";        // Blue 700
        static constexpr const char* PRIMARY_DARK = "#115293";   // Blue 900
        static constexpr const char* PRIMARY_LIGHT = "#42A5F5";  // Blue 400
        static constexpr const char* ACCENT = "#FF4081";         // Pink A200
        static constexpr const char* BACKGROUND = "#FAFAFA";     // Grey 50
        static constexpr const char* SURFACE = "#FFFFFF";        // White
        static constexpr const char* CARD = "#FFFFFF";           // White
        static constexpr const char* TEXT_PRIMARY = "#212121";   // Grey 900
        static constexpr const char* TEXT_SECONDARY = "#757575"; // Grey 600
        static constexpr const char* BORDER = "#E0E0E0";         // Grey 300
        static constexpr const char* HOVER = "#F5F5F5";          // Grey 100
        static constexpr const char* SELECTED = "#E3F2FD";       // Blue 50
        static constexpr const char* ERROR = "#D32F2F";          // Red 700
        static constexpr const char* SUCCESS = "#388E3C";        // Green 700
        static constexpr const char* WARNING = "#F57C00";        // Orange 700
    };
    
    // Dark Theme Colors
    struct DarkTheme {
        static constexpr const char* PRIMARY = "#42A5F5";        // Blue 400
        static constexpr const char* PRIMARY_DARK = "#1976D2";   // Blue 700
        static constexpr const char* PRIMARY_LIGHT = "#90CAF9";  // Blue 200
        static constexpr const char* ACCENT = "#FF4081";         // Pink A200
        static constexpr const char* BACKGROUND = "#121212";     // Material Dark Background
        static constexpr const char* SURFACE = "#1E1E1E";        // Material Dark Surface
        static constexpr const char* CARD = "#2C2C2C";           // Elevated Surface
        static constexpr const char* TEXT_PRIMARY = "#FFFFFF";   // White
        static constexpr const char* TEXT_SECONDARY = "#B0B0B0"; // Grey 400
        static constexpr const char* BORDER = "#424242";         // Grey 800
        static constexpr const char* HOVER = "#2C2C2C";          // Slight elevation
        static constexpr const char* SELECTED = "#1E3A5F";       // Dark blue
        static constexpr const char* ERROR = "#EF5350";          // Red 400
        static constexpr const char* SUCCESS = "#66BB6A";        // Green 400
        static constexpr const char* WARNING = "#FFA726";        // Orange 400
    };
};

#endif // THEME_H

