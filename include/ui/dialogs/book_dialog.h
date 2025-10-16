/**
 * @file book_dialog.h
 * @brief Dialog for adding and editing books
 * 
 * @author Shahzaib Ahmed
 * @date October 16, 2025
 */

#ifndef BOOK_DIALOG_H
#define BOOK_DIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include "core/book.h"

/**
 * @brief Dialog for adding or editing a book
 * 
 * Provides a form interface for entering or modifying book information
 */
class BookDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor for adding a new book
     * @param parent Parent widget
     */
    explicit BookDialog(QWidget *parent = nullptr);
    
    /**
     * @brief Constructor for editing an existing book
     * @param book The book to edit
     * @param parent Parent widget
     */
    explicit BookDialog(const Book& book, QWidget *parent = nullptr);
    
    /**
     * @brief Get the book data from the dialog
     * @return Book object with form data
     */
    Book getBook() const;
    
private slots:
    void onSave();
    void onCancel();
    void validateForm();

private:
    void setupUI();
    void connectSignals();
    void populateForm(const Book& book);
    
    // UI Components
    QLineEdit* m_titleEdit;
    QLineEdit* m_authorEdit;
    QLineEdit* m_isbnEdit;
    QLineEdit* m_genreEdit;
    QLineEdit* m_publisherEdit;
    QSpinBox* m_yearSpin;
    QSpinBox* m_pagesSpin;
    QSpinBox* m_currentPageSpin;
    QComboBox* m_statusCombo;
    QSpinBox* m_ratingSpin;
    QTextEdit* m_notesEdit;
    QTextEdit* m_reviewEdit;
    QPushButton* m_saveButton;
    QPushButton* m_cancelButton;
    
    // Data
    bool m_isEditMode;
    int m_bookId;
};

#endif // BOOK_DIALOG_H

