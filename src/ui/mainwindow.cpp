/**
 * @file mainwindow.cpp
 * @brief Implementation of the Main Window
 * 
 * @author Shahzaib Ahmed
 * @date October 16, 2025
 */

#include "ui/mainwindow.h"
#include "ui/theme.h"
#include "ui/dialogs/book_dialog.h"
#include "core/book_status.h"
#include "utils/string_utils.h"
#include "utils/date_utils.h"
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QInputDialog>
#include <QHeaderView>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QSplitter>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_bookTable(nullptr)
    , m_searchBox(nullptr)
    , m_filterCombo(nullptr)
    , m_addButton(nullptr)
    , m_editButton(nullptr)
    , m_deleteButton(nullptr)
    , m_progressButton(nullptr)
    , m_statsLabel(nullptr)
{
    // Initialize database
    try {
        m_database = std::make_unique<Database>("prms_library.db");
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Database Error", 
            QString("Failed to open database: %1").arg(e.what()));
        return;
    }
    
    setupUI();
    loadBooks();
    updateStatistics();
    connectSignals();
    
    // Apply theme
    Theme::instance().applyTheme();
}

MainWindow::~MainWindow() {
}

void MainWindow::setupUI() {
    setWindowTitle("Personal Reading Management System");
    setMinimumSize(1200, 700);
    
    setupMenuBar();
    setupToolBar();
    setupCentralWidget();
    setupStatusBar();
}

void MainWindow::setupMenuBar() {
    QMenuBar* menuBar = this->menuBar();
    
    // File Menu
    QMenu* fileMenu = menuBar->addMenu("&File");
    
    QAction* importAction = fileMenu->addAction("Import Books...");
    QAction* exportAction = fileMenu->addAction("Export Books...");
    fileMenu->addSeparator();
    QAction* exitAction = fileMenu->addAction("E&xit");
    
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    
    // Edit Menu
    QMenu* editMenu = menuBar->addMenu("&Edit");
    
    QAction* addAction = editMenu->addAction("&Add Book");
    addAction->setShortcut(QKeySequence("Ctrl+N"));
    connect(addAction, &QAction::triggered, this, &MainWindow::onAddBook);
    
    QAction* editAction = editMenu->addAction("&Edit Book");
    editAction->setShortcut(QKeySequence("Ctrl+E"));
    connect(editAction, &QAction::triggered, this, &MainWindow::onEditBook);
    
    QAction* deleteAction = editMenu->addAction("&Delete Book");
    deleteAction->setShortcut(QKeySequence::Delete);
    connect(deleteAction, &QAction::triggered, this, &MainWindow::onDeleteBook);
    
    editMenu->addSeparator();
    
    QAction* refreshAction = editMenu->addAction("&Refresh");
    refreshAction->setShortcut(QKeySequence("F5"));
    connect(refreshAction, &QAction::triggered, this, &MainWindow::onRefresh);
    
    // View Menu
    QMenu* viewMenu = menuBar->addMenu("&View");
    
    QAction* themeAction = viewMenu->addAction("Toggle &Theme");
    themeAction->setShortcut(QKeySequence("Ctrl+T"));
    connect(themeAction, &QAction::triggered, this, &MainWindow::onToggleTheme);
    
    // Help Menu
    QMenu* helpMenu = menuBar->addMenu("&Help");
    
    QAction* aboutAction = helpMenu->addAction("&About");
    connect(aboutAction, &QAction::triggered, [this]() {
        QMessageBox::about(this, "About PRMS",
            "Personal Reading Management System v1.0\n\n"
            "A modern application for tracking your reading journey.\n\n"
            "Built with Qt 6 and SQLite");
    });
}

void MainWindow::setupToolBar() {
    QToolBar* toolbar = addToolBar("Main Toolbar");
    toolbar->setMovable(false);
    toolbar->setIconSize(QSize(24, 24));
    
    // Add Book button
    m_addButton = new QPushButton("Add Book", this);
    toolbar->addWidget(m_addButton);
    
    toolbar->addSeparator();
    
    // Edit Book button
    m_editButton = new QPushButton("Edit", this);
    m_editButton->setEnabled(false);
    toolbar->addWidget(m_editButton);
    
    // Delete Book button
    m_deleteButton = new QPushButton("Delete", this);
    m_deleteButton->setEnabled(false);
    toolbar->addWidget(m_deleteButton);
    
    // Update Progress button
    m_progressButton = new QPushButton("Update Progress", this);
    m_progressButton->setEnabled(false);
    toolbar->addWidget(m_progressButton);
    
    toolbar->addSeparator();
    
    // Spacer
    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    toolbar->addWidget(spacer);
    
    // Theme toggle
    QPushButton* themeButton = new QPushButton("Toggle Theme", this);
    connect(themeButton, &QPushButton::clicked, this, &MainWindow::onToggleTheme);
    toolbar->addWidget(themeButton);
}

void MainWindow::setupCentralWidget() {
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(16);
    
    // Search and Filter Bar
    QHBoxLayout* searchLayout = new QHBoxLayout();
    
    QLabel* searchLabel = new QLabel("Search:", this);
    searchLayout->addWidget(searchLabel);
    
    m_searchBox = new QLineEdit(this);
    m_searchBox->setPlaceholderText("Search by title or author...");
    m_searchBox->setMinimumWidth(300);
    searchLayout->addWidget(m_searchBox);
    
    searchLayout->addSpacing(20);
    
    QLabel* filterLabel = new QLabel("Filter:", this);
    searchLayout->addWidget(filterLabel);
    
    m_filterCombo = new QComboBox(this);
    m_filterCombo->addItem("All Books");
    m_filterCombo->addItem("To Read");
    m_filterCombo->addItem("Reading");
    m_filterCombo->addItem("Completed");
    m_filterCombo->addItem("Did Not Finish");
    m_filterCombo->addItem("Wishlist");
    searchLayout->addWidget(m_filterCombo);
    
    searchLayout->addStretch();
    
    mainLayout->addLayout(searchLayout);
    
    // Book Table
    m_bookTable = new QTableWidget(this);
    m_bookTable->setColumnCount(8);
    m_bookTable->setHorizontalHeaderLabels({
        "ID", "Title", "Author", "Genre", "Pages", "Progress", "Status", "Date Added"
    });
    
    // Configure table
    m_bookTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_bookTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_bookTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_bookTable->setSortingEnabled(true);
    m_bookTable->verticalHeader()->setVisible(false);
    m_bookTable->setAlternatingRowColors(true);
    
    // Set column widths
    m_bookTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    m_bookTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_bookTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    m_bookTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    m_bookTable->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    m_bookTable->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    m_bookTable->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    m_bookTable->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeToContents);
    
    mainLayout->addWidget(m_bookTable);
    
    setCentralWidget(centralWidget);
}

void MainWindow::setupStatusBar() {
    m_statsLabel = new QLabel(this);
    statusBar()->addPermanentWidget(m_statsLabel);
}

void MainWindow::connectSignals() {
    connect(m_addButton, &QPushButton::clicked, this, &MainWindow::onAddBook);
    connect(m_editButton, &QPushButton::clicked, this, &MainWindow::onEditBook);
    connect(m_deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteBook);
    connect(m_progressButton, &QPushButton::clicked, this, &MainWindow::onUpdateProgress);
    
    connect(m_searchBox, &QLineEdit::textChanged, this, &MainWindow::onSearch);
    connect(m_filterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onFilterChanged);
    
    connect(m_bookTable, &QTableWidget::itemSelectionChanged,
            this, &MainWindow::onBookSelectionChanged);
    
    connect(m_bookTable, &QTableWidget::itemDoubleClicked,
            [this]() { onEditBook(); });
}

void MainWindow::loadBooks() {
    try {
        m_allBooks = m_database->getAllBooks();
        m_filteredBooks = m_allBooks;
        
        // Update table
        m_bookTable->setSortingEnabled(false);
        m_bookTable->setRowCount(0);
        
        for (const auto& book : m_filteredBooks) {
            int row = m_bookTable->rowCount();
            m_bookTable->insertRow(row);
            
            // ID
            m_bookTable->setItem(row, 0, new QTableWidgetItem(QString::number(book.getId())));
            
            // Title
            m_bookTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(book.getTitle())));
            
            // Author
            m_bookTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(book.getAuthor())));
            
            // Genre
            QString genre = QString::fromStdString(book.getGenre());
            if (genre.isEmpty()) genre = "—";
            m_bookTable->setItem(row, 3, new QTableWidgetItem(genre));
            
            // Pages
            m_bookTable->setItem(row, 4, new QTableWidgetItem(QString::number(book.getPageCount())));
            
            // Progress
            QString progress = QString("%1 / %2 (%3%)")
                .arg(book.getCurrentPage())
                .arg(book.getPageCount())
                .arg(QString::number(book.getProgressPercentage(), 'f', 1));
            m_bookTable->setItem(row, 5, new QTableWidgetItem(progress));
            
            // Status
            QString status = QString::fromStdString(
                bookStatusToString(intToBookStatus(book.getStatus())));
            m_bookTable->setItem(row, 6, new QTableWidgetItem(status));
            
            // Date Added
            QString dateAdded = QString::fromStdString(
                DateUtils::formatShortDate(book.getDateAdded()));
            m_bookTable->setItem(row, 7, new QTableWidgetItem(dateAdded));
        }
        
        m_bookTable->setSortingEnabled(true);
        
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", 
            QString("Failed to load books: %1").arg(e.what()));
    }
}

void MainWindow::updateStatistics() {
    int total = m_allBooks.size();
    int reading = 0, completed = 0, toRead = 0;
    
    for (const auto& book : m_allBooks) {
        BookStatus status = intToBookStatus(book.getStatus());
        switch (status) {
            case BookStatus::Reading: reading++; break;
            case BookStatus::Completed: completed++; break;
            case BookStatus::ToRead: toRead++; break;
            default: break;
        }
    }
    
    QString stats = QString("Total: %1  |  Reading: %2  |  Completed: %3  |  To Read: %4")
        .arg(total).arg(reading).arg(completed).arg(toRead);
    
    m_statsLabel->setText(stats);
}

void MainWindow::onAddBook() {
    BookDialog dialog(this);
    
    if (dialog.exec() == QDialog::Accepted) {
        try {
            Book book = dialog.getBook();
            m_database->addBook(book);
            
            loadBooks();
            updateStatistics();
            
            QMessageBox::information(this, "Success", 
                QString("Book '%1' added successfully!")
                .arg(QString::fromStdString(book.getTitle())));
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error",
                QString("Failed to add book: %1").arg(e.what()));
        }
    }
}

void MainWindow::onEditBook() {
    QList<QTableWidgetItem*> selected = m_bookTable->selectedItems();
    if (selected.isEmpty()) return;
    
    int row = selected[0]->row();
    int bookId = m_bookTable->item(row, 0)->text().toInt();
    
    auto bookOpt = m_database->getBook(bookId);
    if (!bookOpt.has_value()) {
        QMessageBox::warning(this, "Error", "Book not found!");
        return;
    }
    
    Book book = bookOpt.value();
    BookDialog dialog(book, this);
    
    if (dialog.exec() == QDialog::Accepted) {
        try {
            Book updatedBook = dialog.getBook();
            m_database->updateBook(updatedBook);
            
            loadBooks();
            updateStatistics();
            
            QMessageBox::information(this, "Success", 
                QString("Book '%1' updated successfully!")
                .arg(QString::fromStdString(updatedBook.getTitle())));
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error",
                QString("Failed to update book: %1").arg(e.what()));
        }
    }
}

void MainWindow::onDeleteBook() {
    QList<QTableWidgetItem*> selected = m_bookTable->selectedItems();
    if (selected.isEmpty()) return;
    
    int row = selected[0]->row();
    int bookId = m_bookTable->item(row, 0)->text().toInt();
    QString title = m_bookTable->item(row, 1)->text();
    
    QMessageBox::StandardButton reply = QMessageBox::question(this,
        "Confirm Delete",
        QString("Are you sure you want to delete '%1'?\n\nThis cannot be undone.").arg(title),
        QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        try {
            m_database->deleteBook(bookId);
            loadBooks();
            updateStatistics();
            
            QMessageBox::information(this, "Success", "Book deleted successfully!");
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error",
                QString("Failed to delete book: %1").arg(e.what()));
        }
    }
}

void MainWindow::onUpdateProgress() {
    QList<QTableWidgetItem*> selected = m_bookTable->selectedItems();
    if (selected.isEmpty()) return;
    
    int row = selected[0]->row();
    int bookId = m_bookTable->item(row, 0)->text().toInt();
    
    auto bookOpt = m_database->getBook(bookId);
    if (!bookOpt.has_value()) {
        QMessageBox::warning(this, "Error", "Book not found!");
        return;
    }
    
    Book book = bookOpt.value();
    
    bool ok;
    int newPage = QInputDialog::getInt(this, "Update Progress",
        QString("Current page (out of %1):").arg(book.getPageCount()),
        book.getCurrentPage(), 0, book.getPageCount(), 1, &ok);
    
    if (ok) {
        try {
            book.setCurrentPage(newPage);
            m_database->updateBook(book);
            
            loadBooks();
            updateStatistics();
            
            QMessageBox::information(this, "Success",
                QString("Progress updated: %1%").arg(
                    QString::number(book.getProgressPercentage(), 'f', 1)));
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error",
                QString("Failed to update progress: %1").arg(e.what()));
        }
    }
}

void MainWindow::onRefresh() {
    loadBooks();
    updateStatistics();
}

void MainWindow::onSearch(const QString& text) {
    if (text.isEmpty()) {
        m_filteredBooks = m_allBooks;
    } else {
        try {
            m_filteredBooks = m_database->searchBooks(text.toStdString());
        } catch (const std::exception& e) {
            QMessageBox::critical(this, "Error",
                QString("Search failed: %1").arg(e.what()));
            return;
        }
    }
    
    // Apply filter if active
    int filterIndex = m_filterCombo->currentIndex();
    if (filterIndex > 0) {
        onFilterChanged(filterIndex);
    } else {
        loadBooks();
    }
}

void MainWindow::onFilterChanged(int index) {
    std::vector<Book> filtered;
    
    if (index == 0) {
        // All books
        m_filteredBooks = m_allBooks;
    } else {
        // Filter by status
        BookStatus targetStatus = static_cast<BookStatus>(index - 1);
        
        for (const auto& book : m_allBooks) {
            if (intToBookStatus(book.getStatus()) == targetStatus) {
                filtered.push_back(book);
            }
        }
        
        m_filteredBooks = filtered;
    }
    
    loadBooks();
}

void MainWindow::onToggleTheme() {
    Theme::instance().toggleTheme();
}

void MainWindow::onBookSelectionChanged() {
    bool hasSelection = !m_bookTable->selectedItems().isEmpty();
    m_editButton->setEnabled(hasSelection);
    m_deleteButton->setEnabled(hasSelection);
    m_progressButton->setEnabled(hasSelection);
}

