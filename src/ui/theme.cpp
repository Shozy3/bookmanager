/**
 * @file theme.cpp
 * @brief Implementation of the Theme manager
 * 
 * @author Shahzaib Ahmed
 * @date October 16, 2025
 */

#include "ui/theme.h"
#include <QSettings>
#include <QFile>

// ==== Singleton Implementation ====

Theme& Theme::instance() {
    static Theme instance;
    return instance;
}

Theme::Theme()
    : m_currentMode(ThemeMode::Light)
{
    loadThemeFromSettings();
}

// ==== Theme Mode Management ====

ThemeMode Theme::getCurrentMode() const {
    return m_currentMode;
}

void Theme::setThemeMode(ThemeMode mode) {
    if (m_currentMode != mode) {
        m_currentMode = mode;
        saveThemeToSettings();
        applyTheme();
    }
}

void Theme::toggleTheme() {
    if (m_currentMode == ThemeMode::Light) {
        setThemeMode(ThemeMode::Dark);
    } else {
        setThemeMode(ThemeMode::Light);
    }
}

void Theme::applyTheme() {
    QString stylesheet = generateStylesheet();
    qApp->setStyleSheet(stylesheet);
}

// ==== Settings Persistence ====

void Theme::loadThemeFromSettings() {
    QSettings settings("PRMS", "PersonalReadingManager");
    QString themeStr = settings.value("theme/mode", "light").toString();
    
    if (themeStr == "dark") {
        m_currentMode = ThemeMode::Dark;
    } else {
        m_currentMode = ThemeMode::Light;
    }
}

void Theme::saveThemeToSettings() {
    QSettings settings("PRMS", "PersonalReadingManager");
    QString themeStr = (m_currentMode == ThemeMode::Dark) ? "dark" : "light";
    settings.setValue("theme/mode", themeStr);
}

// ==== Color Accessors ====

QColor Theme::getPrimaryColor() const {
    return QColor(m_currentMode == ThemeMode::Light ? 
                 LightTheme::PRIMARY : DarkTheme::PRIMARY);
}

QColor Theme::getPrimaryDarkColor() const {
    return QColor(m_currentMode == ThemeMode::Light ? 
                 LightTheme::PRIMARY_DARK : DarkTheme::PRIMARY_DARK);
}

QColor Theme::getPrimaryLightColor() const {
    return QColor(m_currentMode == ThemeMode::Light ? 
                 LightTheme::PRIMARY_LIGHT : DarkTheme::PRIMARY_LIGHT);
}

QColor Theme::getAccentColor() const {
    return QColor(m_currentMode == ThemeMode::Light ? 
                 LightTheme::ACCENT : DarkTheme::ACCENT);
}

QColor Theme::getBackgroundColor() const {
    return QColor(m_currentMode == ThemeMode::Light ? 
                 LightTheme::BACKGROUND : DarkTheme::BACKGROUND);
}

QColor Theme::getSurfaceColor() const {
    return QColor(m_currentMode == ThemeMode::Light ? 
                 LightTheme::SURFACE : DarkTheme::SURFACE);
}

QColor Theme::getCardColor() const {
    return QColor(m_currentMode == ThemeMode::Light ? 
                 LightTheme::CARD : DarkTheme::CARD);
}

QColor Theme::getTextPrimaryColor() const {
    return QColor(m_currentMode == ThemeMode::Light ? 
                 LightTheme::TEXT_PRIMARY : DarkTheme::TEXT_PRIMARY);
}

QColor Theme::getTextSecondaryColor() const {
    return QColor(m_currentMode == ThemeMode::Light ? 
                 LightTheme::TEXT_SECONDARY : DarkTheme::TEXT_SECONDARY);
}

QColor Theme::getBorderColor() const {
    return QColor(m_currentMode == ThemeMode::Light ? 
                 LightTheme::BORDER : DarkTheme::BORDER);
}

QColor Theme::getHoverColor() const {
    return QColor(m_currentMode == ThemeMode::Light ? 
                 LightTheme::HOVER : DarkTheme::HOVER);
}

QColor Theme::getSelectedColor() const {
    return QColor(m_currentMode == ThemeMode::Light ? 
                 LightTheme::SELECTED : DarkTheme::SELECTED);
}

QColor Theme::getErrorColor() const {
    return QColor(m_currentMode == ThemeMode::Light ? 
                 LightTheme::ERROR : DarkTheme::ERROR);
}

QColor Theme::getSuccessColor() const {
    return QColor(m_currentMode == ThemeMode::Light ? 
                 LightTheme::SUCCESS : DarkTheme::SUCCESS);
}

QColor Theme::getWarningColor() const {
    return QColor(m_currentMode == ThemeMode::Light ? 
                 LightTheme::WARNING : DarkTheme::WARNING);
}

// ==== Stylesheet Generation ====

QString Theme::generateStylesheet() const {
    const char* primary = (m_currentMode == ThemeMode::Light) ? 
                         LightTheme::PRIMARY : DarkTheme::PRIMARY;
    const char* primaryDark = (m_currentMode == ThemeMode::Light) ? 
                             LightTheme::PRIMARY_DARK : DarkTheme::PRIMARY_DARK;
    const char* accent = (m_currentMode == ThemeMode::Light) ? 
                        LightTheme::ACCENT : DarkTheme::ACCENT;
    const char* background = (m_currentMode == ThemeMode::Light) ? 
                            LightTheme::BACKGROUND : DarkTheme::BACKGROUND;
    const char* surface = (m_currentMode == ThemeMode::Light) ? 
                         LightTheme::SURFACE : DarkTheme::SURFACE;
    const char* card = (m_currentMode == ThemeMode::Light) ? 
                      LightTheme::CARD : DarkTheme::CARD;
    const char* textPrimary = (m_currentMode == ThemeMode::Light) ? 
                             LightTheme::TEXT_PRIMARY : DarkTheme::TEXT_PRIMARY;
    const char* textSecondary = (m_currentMode == ThemeMode::Light) ? 
                               LightTheme::TEXT_SECONDARY : DarkTheme::TEXT_SECONDARY;
    const char* border = (m_currentMode == ThemeMode::Light) ? 
                        LightTheme::BORDER : DarkTheme::BORDER;
    const char* hover = (m_currentMode == ThemeMode::Light) ? 
                       LightTheme::HOVER : DarkTheme::HOVER;
    const char* selected = (m_currentMode == ThemeMode::Light) ? 
                          LightTheme::SELECTED : DarkTheme::SELECTED;
    
    QString stylesheet = QString(R"(
        /* Main Application */
        QMainWindow {
            background-color: %1;
        }
        
        /* General Widget Styling */
        QWidget {
            font-family: "Segoe UI", "Roboto", "Arial", sans-serif;
            font-size: 10pt;
            color: %8;
        }
        
        /* Buttons */
        QPushButton {
            background-color: %2;
            color: white;
            border: none;
            padding: 8px 16px;
            border-radius: 4px;
            font-weight: 500;
            min-height: 32px;
        }
        
        QPushButton:hover {
            background-color: %3;
        }
        
        QPushButton:pressed {
            background-color: %3;
        }
        
        QPushButton:disabled {
            background-color: %10;
            color: %9;
        }
        
        /* Text Input Fields */
        QLineEdit, QTextEdit, QPlainTextEdit {
            background-color: %7;
            border: 1px solid %10;
            border-radius: 4px;
            padding: 8px;
            selection-background-color: %2;
            color: %8;
        }
        
        QLineEdit:focus, QTextEdit:focus, QPlainTextEdit:focus {
            border: 2px solid %2;
        }
        
        /* Combo Box */
        QComboBox {
            background-color: %7;
            border: 1px solid %10;
            border-radius: 4px;
            padding: 6px;
            min-height: 28px;
            color: %8;
        }
        
        QComboBox:hover {
            border-color: %2;
        }
        
        QComboBox::drop-down {
            border: none;
            padding-right: 8px;
        }
        
        QComboBox QAbstractItemView {
            background-color: %7;
            border: 1px solid %10;
            selection-background-color: %11;
            color: %8;
        }
        
        /* List Widget */
        QListWidget {
            background-color: %7;
            border: 1px solid %10;
            border-radius: 4px;
            padding: 4px;
            color: %8;
        }
        
        QListWidget::item {
            padding: 8px;
            border-radius: 4px;
        }
        
        QListWidget::item:hover {
            background-color: %11;
        }
        
        QListWidget::item:selected {
            background-color: %12;
            color: %8;
        }
        
        /* Table Widget */
        QTableWidget {
            background-color: %7;
            gridline-color: %10;
            border: 1px solid %10;
            border-radius: 4px;
            color: %8;
        }
        
        QTableWidget::item {
            padding: 8px;
        }
        
        QTableWidget::item:selected {
            background-color: %12;
            color: %8;
        }
        
        QHeaderView::section {
            background-color: %6;
            padding: 8px;
            border: none;
            border-bottom: 2px solid %10;
            font-weight: 600;
            color: %8;
        }
        
        /* Scroll Bars */
        QScrollBar:vertical {
            background-color: %1;
            width: 12px;
            margin: 0;
        }
        
        QScrollBar::handle:vertical {
            background-color: %10;
            border-radius: 6px;
            min-height: 20px;
        }
        
        QScrollBar::handle:vertical:hover {
            background-color: %9;
        }
        
        QScrollBar:horizontal {
            background-color: %1;
            height: 12px;
            margin: 0;
        }
        
        QScrollBar::handle:horizontal {
            background-color: %10;
            border-radius: 6px;
            min-width: 20px;
        }
        
        QScrollBar::handle:horizontal:hover {
            background-color: %9;
        }
        
        QScrollBar::add-line, QScrollBar::sub-line {
            border: none;
            background: none;
        }
        
        /* Progress Bar */
        QProgressBar {
            border: none;
            border-radius: 4px;
            background-color: %10;
            text-align: center;
            color: %8;
            height: 20px;
        }
        
        QProgressBar::chunk {
            background-color: %2;
            border-radius: 4px;
        }
        
        /* Labels */
        QLabel {
            color: %8;
        }
        
        /* Menus */
        QMenuBar {
            background-color: %6;
            color: %8;
            padding: 4px;
        }
        
        QMenuBar::item:selected {
            background-color: %11;
        }
        
        QMenu {
            background-color: %7;
            border: 1px solid %10;
            padding: 4px;
            color: %8;
        }
        
        QMenu::item {
            padding: 8px 24px;
            border-radius: 4px;
        }
        
        QMenu::item:selected {
            background-color: %11;
        }
        
        /* Tabs */
        QTabWidget::pane {
            border: 1px solid %10;
            background-color: %7;
            border-radius: 4px;
        }
        
        QTabBar::tab {
            background-color: %6;
            color: %9;
            padding: 10px 20px;
            border-top-left-radius: 4px;
            border-top-right-radius: 4px;
            margin-right: 2px;
        }
        
        QTabBar::tab:selected {
            background-color: %7;
            color: %2;
            font-weight: 600;
        }
        
        QTabBar::tab:hover:!selected {
            background-color: %11;
        }
        
        /* Tool Bar */
        QToolBar {
            background-color: %6;
            border: none;
            padding: 4px;
            spacing: 4px;
        }
        
        QToolButton {
            background-color: transparent;
            border: none;
            padding: 8px;
            border-radius: 4px;
            color: %8;
        }
        
        QToolButton:hover {
            background-color: %11;
        }
        
        QToolButton:pressed {
            background-color: %12;
        }
        
        /* Status Bar */
        QStatusBar {
            background-color: %6;
            color: %9;
        }
        
        /* Spin Box */
        QSpinBox, QDoubleSpinBox {
            background-color: %7;
            border: 1px solid %10;
            border-radius: 4px;
            padding: 6px;
            color: %8;
        }
        
        QSpinBox:focus, QDoubleSpinBox:focus {
            border-color: %2;
        }
        
        /* Check Box and Radio Button */
        QCheckBox, QRadioButton {
            color: %8;
            spacing: 8px;
        }
        
        QCheckBox::indicator, QRadioButton::indicator {
            width: 18px;
            height: 18px;
        }
        
        QCheckBox::indicator:checked {
            background-color: %2;
            border: 2px solid %2;
            border-radius: 3px;
        }
        
        QCheckBox::indicator:unchecked {
            background-color: %7;
            border: 2px solid %10;
            border-radius: 3px;
        }
        
        /* Group Box */
        QGroupBox {
            border: 1px solid %10;
            border-radius: 4px;
            margin-top: 12px;
            padding-top: 12px;
            font-weight: 600;
            color: %8;
        }
        
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 12px;
            padding: 0 4px;
        }
        
    )").arg(background)     // %1 - background
       .arg(primary)        // %2 - primary
       .arg(primaryDark)    // %3 - primary dark
       .arg(accent)         // %4 - accent (not used in this stylesheet yet)
       .arg("")             // %5 - reserved
       .arg(surface)        // %6 - surface
       .arg(card)           // %7 - card
       .arg(textPrimary)    // %8 - text primary
       .arg(textSecondary)  // %9 - text secondary
       .arg(border)         // %10 - border
       .arg(hover)          // %11 - hover
       .arg(selected);      // %12 - selected
    
    return stylesheet;
}

