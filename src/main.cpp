/**
 * @file main.cpp
 * @brief Main entry point for the Personal Reading Management System (PRMS)
 * 
 * This file contains the main function that initializes the Qt application
 * and starts the GUI. It serves as the entry point for the entire application.
 * 
 * @author Shahzaib Ahmed
 * @date October 12, 2025
 */

#include <QApplication>
#include <QStyleFactory>
#include <QPalette>
#include <QDir>
#include <QStandardPaths>
#include <QDebug>
#include <QTimer>

// Forward declarations - we'll implement these in future commits
class MainWindow;

/**
 * @brief Sets up the application's appearance and theme
 * 
 * This function configures the application to use a modern, clean appearance
 * with support for both light and dark themes. It sets up the Qt style
 * and palette to provide a professional look.
 * 
 * @param app Reference to the QApplication instance
 */
void setupApplicationTheme(QApplication& app) {
    // Set application style to modern look
    app.setStyle(QStyleFactory::create("Fusion"));
    
    // Create a custom palette for better appearance
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(53, 53, 53));
    palette.setColor(QPalette::WindowText, Qt::white);
    palette.setColor(QPalette::Base, QColor(25, 25, 25));
    palette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
    palette.setColor(QPalette::ToolTipBase, Qt::white);
    palette.setColor(QPalette::ToolTipText, Qt::white);
    palette.setColor(QPalette::Text, Qt::white);
    palette.setColor(QPalette::Button, QColor(53, 53, 53));
    palette.setColor(QPalette::ButtonText, Qt::white);
    palette.setColor(QPalette::BrightText, Qt::red);
    palette.setColor(QPalette::Link, QColor(42, 130, 218));
    palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    palette.setColor(QPalette::HighlightedText, Qt::black);
    
    app.setPalette(palette);
}

/**
 * @brief Creates necessary application directories
 * 
 * This function ensures that all required directories exist for the application
 * to function properly. It creates directories for data storage, configuration,
 * and temporary files in the appropriate system locations.
 */
void createApplicationDirectories() {
    // Get application data directory
    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString configDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    
    // Create directories if they don't exist
    QDir().mkpath(dataDir);
    QDir().mkpath(configDir);
    
    qDebug() << "Application data directory:" << dataDir;
    qDebug() << "Application config directory:" << configDir;
}

/**
 * @brief Main entry point of the application
 * 
 * This function initializes the Qt application, sets up the theme,
 * creates necessary directories, and starts the main window.
 * 
 * @param argc Number of command line arguments
 * @param argv Array of command line arguments
 * @return Exit code (0 for success, non-zero for error)
 */
int main(int argc, char *argv[]) {
    // Create Qt application instance
    QApplication app(argc, argv);
    
    // Set application metadata
    app.setApplicationName("Personal Reading Management System");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("PRMS");
    app.setOrganizationDomain("prms.local");
    
    // Setup application appearance
    setupApplicationTheme(app);
    
    // Create necessary directories
    createApplicationDirectories();
    
    // Print startup information
    qDebug() << "=== PRMS Starting ===";
    qDebug() << "Application Name:" << app.applicationName();
    qDebug() << "Application Version:" << app.applicationVersion();
    qDebug() << "Qt Version:" << QT_VERSION_STR;
    
    qDebug() << "PRMS initialized successfully!";
    qDebug() << "Main window will be implemented in the next commit.";
    
    QTimer::singleShot(1000, &app, &QApplication::quit);
    
    // Start the application event loop
    return app.exec();
}
