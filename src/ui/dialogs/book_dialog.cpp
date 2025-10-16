/**
 * @file book_dialog.cpp
 * @brief Implementation of BookDialog
 * 
 * @author Shahzaib Ahmed
 * @date October 16, 2025
 */

#include "ui/dialogs/book_dialog.h"
#include "core/book_status.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QTabWidget>
#include <QMessageBox>
#include <QDateTime>

BookDialog::BookDialog(QWidget *parent)
    : QDialog(parent)
    , m_isEditMode(false)
    , m_bookId(0)
{
    setupUI();
    connectSignals();
    setWindowTitle("Add New Book");
}

BookDialog::BookDialog(const Book& book, QWidget *parent)
    : QDialog(parent)
    , m_isEditMode(true)
    , m_bookId(book.getId())
{
    setupUI();
    connectSignals();
    populateForm(book);
    setWindowTitle("Edit Book");
}

void BookDialog::setupUI() {
    setMinimumSize(600, 700);
    
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(16);
    
    // Create tab widget for organization
    QTabWidget* tabWidget = new QTabWidget(this);
    
    // === BASIC INFO TAB ===
    QWidget* basicTab = new QWidget();
    QVBoxLayout* basicLayout = new QVBoxLayout(basicTab);
    basicLayout->setSpacing(12);
    
    QFormLayout* basicForm = new QFormLayout();
    basicForm->setSpacing(10);
    
    // Title (required)
    m_titleEdit = new QLineEdit();
    m_titleEdit->setPlaceholderText("Enter book title...");
    basicForm->addRow("Title *:", m_titleEdit);
    
    // Author (required)
    m_authorEdit = new QLineEdit();
    m_authorEdit->setPlaceholderText("Enter author name...");
    basicForm->addRow("Author *:", m_authorEdit);
    
    // ISBN (optional)
    m_isbnEdit = new QLineEdit();
    m_isbnEdit->setPlaceholderText("13-digit ISBN (optional)");
    m_isbnEdit->setMaxLength(17); // Allow for hyphens
    basicForm->addRow("ISBN:", m_isbnEdit);
    
    // Genre
    m_genreEdit = new QLineEdit();
    m_genreEdit->setPlaceholderText("e.g., Fiction, Mystery, Science...");
    basicForm->addRow("Genre:", m_genreEdit);
    
    // Publisher
    m_publisherEdit = new QLineEdit();
    m_publisherEdit->setPlaceholderText("Publisher name...");
    basicForm->addRow("Publisher:", m_publisherEdit);
    
    // Year Published
    m_yearSpin = new QSpinBox();
    m_yearSpin->setRange(0, QDateTime::currentDateTime().date().year());
    m_yearSpin->setValue(0);
    m_yearSpin->setSpecialValueText("Unknown");
    basicForm->addRow("Year Published:", m_yearSpin);
    
    // Page Count (required)
    m_pagesSpin = new QSpinBox();
    m_pagesSpin->setRange(1, 100000);
    m_pagesSpin->setValue(300);
    m_pagesSpin->setSuffix(" pages");
    basicForm->addRow("Page Count *:", m_pagesSpin);
    
    basicLayout->addLayout(basicForm);
    basicLayout->addStretch();
    tabWidget->addTab(basicTab, "Basic Info");
    
    // === PROGRESS TAB ===
    QWidget* progressTab = new QWidget();
    QVBoxLayout* progressLayout = new QVBoxLayout(progressTab);
    progressLayout->setSpacing(12);
    
    QFormLayout* progressForm = new QFormLayout();
    progressForm->setSpacing(10);
    
    // Current Page
    m_currentPageSpin = new QSpinBox();
    m_currentPageSpin->setRange(0, 100000);
    m_currentPageSpin->setValue(0);
    m_currentPageSpin->setSuffix(" pages");
    progressForm->addRow("Current Page:", m_currentPageSpin);
    
    // Status
    m_statusCombo = new QComboBox();
    for (const auto& statusName : getAllBookStatusNames()) {
        m_statusCombo->addItem(QString::fromStdString(statusName));
    }
    progressForm->addRow("Status:", m_statusCombo);
    
    // Rating
    m_ratingSpin = new QSpinBox();
    m_ratingSpin->setRange(0, 5);
    m_ratingSpin->setValue(0);
    m_ratingSpin->setSpecialValueText("Not Rated");
    m_ratingSpin->setSuffix(" stars");
    progressForm->addRow("Rating:", m_ratingSpin);
    
    progressLayout->addLayout(progressForm);
    progressLayout->addStretch();
    tabWidget->addTab(progressTab, "Progress & Rating");
    
    // === NOTES & REVIEW TAB ===
    QWidget* notesTab = new QWidget();
    QVBoxLayout* notesLayout = new QVBoxLayout(notesTab);
    notesLayout->setSpacing(12);
    
    QLabel* notesLabel = new QLabel("Personal Notes:");
    notesLabel->setStyleSheet("font-weight: bold;");
    notesLayout->addWidget(notesLabel);
    
    m_notesEdit = new QTextEdit();
    m_notesEdit->setPlaceholderText("Add your thoughts, quotes, or observations...");
    m_notesEdit->setMaximumHeight(150);
    notesLayout->addWidget(m_notesEdit);
    
    QLabel* reviewLabel = new QLabel("Review:");
    reviewLabel->setStyleSheet("font-weight: bold;");
    notesLayout->addWidget(reviewLabel);
    
    m_reviewEdit = new QTextEdit();
    m_reviewEdit->setPlaceholderText("Write your review of the book...");
    m_reviewEdit->setMaximumHeight(200);
    notesLayout->addWidget(m_reviewEdit);
    
    notesLayout->addStretch();
    tabWidget->addTab(notesTab, "Notes & Review");
    
    mainLayout->addWidget(tabWidget);
    
    // === BUTTONS ===
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    
    m_cancelButton = new QPushButton("Cancel");
    buttonLayout->addWidget(m_cancelButton);
    
    m_saveButton = new QPushButton(m_isEditMode ? "Update" : "Add Book");
    m_saveButton->setDefault(true);
    buttonLayout->addWidget(m_saveButton);
    
    mainLayout->addLayout(buttonLayout);
    
    // Add required field note
    QLabel* reqLabel = new QLabel("* Required fields");
    reqLabel->setStyleSheet("color: #999; font-size: 9pt;");
    mainLayout->addWidget(reqLabel);
}

void BookDialog::connectSignals() {
    connect(m_saveButton, &QPushButton::clicked, this, &BookDialog::onSave);
    connect(m_cancelButton, &QPushButton::clicked, this, &BookDialog::onCancel);
    
    // Validate form on changes
    connect(m_titleEdit, &QLineEdit::textChanged, this, &BookDialog::validateForm);
    connect(m_authorEdit, &QLineEdit::textChanged, this, &BookDialog::validateForm);
    
    // Sync current page with total pages
    connect(m_pagesSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            [this](int value) {
                m_currentPageSpin->setMaximum(value);
                if (m_currentPageSpin->value() > value) {
                    m_currentPageSpin->setValue(value);
                }
            });
    
    validateForm(); // Initial validation
}

void BookDialog::populateForm(const Book& book) {
    m_titleEdit->setText(QString::fromStdString(book.getTitle()));
    m_authorEdit->setText(QString::fromStdString(book.getAuthor()));
    m_isbnEdit->setText(QString::fromStdString(book.getISBN()));
    m_genreEdit->setText(QString::fromStdString(book.getGenre()));
    m_publisherEdit->setText(QString::fromStdString(book.getPublisher()));
    m_yearSpin->setValue(book.getYearPublished());
    m_pagesSpin->setValue(book.getPageCount());
    m_currentPageSpin->setMaximum(book.getPageCount());
    m_currentPageSpin->setValue(book.getCurrentPage());
    m_statusCombo->setCurrentIndex(book.getStatus());
    m_ratingSpin->setValue(book.getRating());
    m_notesEdit->setPlainText(QString::fromStdString(book.getNotes()));
    m_reviewEdit->setPlainText(QString::fromStdString(book.getReview()));
}

void BookDialog::validateForm() {
    bool valid = !m_titleEdit->text().trimmed().isEmpty() &&
                 !m_authorEdit->text().trimmed().isEmpty();
    
    m_saveButton->setEnabled(valid);
}

Book BookDialog::getBook() const {
    Book book;
    
    if (m_isEditMode) {
        book.setId(m_bookId);
    }
    
    book.setTitle(m_titleEdit->text().trimmed().toStdString());
    book.setAuthor(m_authorEdit->text().trimmed().toStdString());
    book.setISBN(m_isbnEdit->text().trimmed().toStdString());
    book.setGenre(m_genreEdit->text().trimmed().toStdString());
    book.setPublisher(m_publisherEdit->text().trimmed().toStdString());
    book.setYearPublished(m_yearSpin->value());
    book.setPageCount(m_pagesSpin->value());
    book.setCurrentPage(m_currentPageSpin->value());
    book.setStatus(m_statusCombo->currentIndex());
    book.setRating(m_ratingSpin->value());
    book.setNotes(m_notesEdit->toPlainText().toStdString());
    book.setReview(m_reviewEdit->toPlainText().toStdString());
    
    return book;
}

void BookDialog::onSave() {
    // Validate ISBN if provided
    QString isbn = m_isbnEdit->text().trimmed();
    if (!isbn.isEmpty()) {
        // Remove hyphens for validation
        QString cleanIsbn = isbn;
        cleanIsbn.remove('-').remove(' ');
        
        if (cleanIsbn.length() != 13) {
            QMessageBox::warning(this, "Invalid ISBN",
                "ISBN must be exactly 13 digits. Please check and try again.");
            m_isbnEdit->setFocus();
            return;
        }
        
        // Check all digits
        bool allDigits = true;
        for (const QChar& c : cleanIsbn) {
            if (!c.isDigit()) {
                allDigits = false;
                break;
            }
        }
        
        if (!allDigits) {
            QMessageBox::warning(this, "Invalid ISBN",
                "ISBN must contain only digits. Please check and try again.");
            m_isbnEdit->setFocus();
            return;
        }
    }
    
    accept();
}

void BookDialog::onCancel() {
    reject();
}

