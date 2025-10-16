/**
 * @file mainwindow.h
 * @brief Main window for the Personal Reading Management System
 * 
 * @author Shahzaib Ahmed
 * @date October 16, 2025
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <memory>
#include "core/database.h"
#include "core/book.h"

/**
 * @brief Main application window
 * 
 * Provides the primary user interface for managing books
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddBook();
    void onEditBook();
    void onDeleteBook();
    void onUpdateProgress();
    void onRefresh();
    void onSearch(const QString& text);
    void onFilterChanged(int index);
    void onToggleTheme();
    void onBookSelectionChanged();

private:
    void setupUI();
    void setupMenuBar();
    void setupToolBar();
    void setupCentralWidget();
    void setupStatusBar();
    void loadBooks();
    void updateStatistics();
    void connectSignals();
    
    // UI Components
    QTableWidget* m_bookTable;
    QLineEdit* m_searchBox;
    QComboBox* m_filterCombo;
    QPushButton* m_addButton;
    QPushButton* m_editButton;
    QPushButton* m_deleteButton;
    QPushButton* m_progressButton;
    QLabel* m_statsLabel;
    
    // Data
    std::unique_ptr<Database> m_database;
    std::vector<Book> m_allBooks;
    std::vector<Book> m_filteredBooks;
};

#endif // MAINWINDOW_H

