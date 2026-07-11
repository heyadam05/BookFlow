#pragma once

#include "FileManager.h"
#include "Logger.h"
#include "Optional.h"

struct OperationResult {
    bool success = false;
    std::string message;
};

class Library {
public:
    explicit Library(fs::path dataDirectory = "data");

    void load();
    void save();
    void seedDefaultsIfEmpty();
    void refreshOverdueLoans();

    OperationResult addBook(Book book);
    OperationResult editBook(int id, const std::string& field, const std::string& value);
    OperationResult deleteBook(int id);
    OperationResult addReader(Reader reader);
    OperationResult createLoan(int bookId, int readerId, int days);
    OperationResult returnLoanByBook(int bookId);
    OperationResult createReservation(int bookId, int readerId);

    Optional<Book> findBook(int id) const;
    Optional<Reader> findReader(int id) const;
    std::vector<Book> searchBooks(const std::string& query) const;
    std::vector<Book> filterBooks(const std::vector<std::string>& args) const;
    fs::path generateReport() const;
    OperationResult exportBooks(const std::string& format, const fs::path& path) const;

    const std::vector<Book>& books() const;
    const std::vector<Reader>& readers() const;
    const std::vector<Loan>& loans() const;
    const std::vector<Reservation>& reservations() const;
    const std::vector<Category>& categories() const;
    const Settings& settings() const;
    std::string statsText() const;
    int activeLoanCountForReader(int readerId) const;
    int reservationCountForReader(int readerId) const;
    int loanHistoryCountForReader(int readerId) const;

private:
    int nextBookId() const;
    int nextReaderId() const;
    int nextLoanId() const;
    int nextReservationId() const;
    Book* getBook(int id);
    Reader* getReader(int id);
    Loan* getActiveLoanForBook(int bookId);
    void persistAfterChange(const std::string& logMessage);

    FileManager fileManager_;
    Logger logger_;
    LibraryData data_;
};
