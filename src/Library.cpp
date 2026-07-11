#include "Library.h"

#include "ReportGenerator.h"
#include "SearchEngine.h"
#include "Utils.h"
#include <algorithm>

Library::Library(fs::path dataDirectory)
    : fileManager_(dataDirectory), logger_(dataDirectory / "logs.txt") {}

void Library::load() {
    data_ = fileManager_.load();
    seedDefaultsIfEmpty();
    refreshOverdueLoans();
}

void Library::save() {
    fileManager_.save(data_);
}

void Library::seedDefaultsIfEmpty() {
    if (data_.categories.empty()) {
        const std::vector<std::string> names = {"Programming", "Fantasy", "Science Fiction", "History",
            "Biography", "Education", "Psychology", "Business", "Other"};
        int id = 1;
        for (const auto& name : names) data_.categories.push_back({id++, name, "Default category"});
    }
    save();
}

void Library::refreshOverdueLoans() {
    bool changed = false;
    for (auto& loan : data_.loans) {
        if (loan.status == LoanStatus::Active && Utils::daysBetween(loan.dueDate, Utils::today()) > 0) {
            loan.status = LoanStatus::Overdue;
            changed = true;
        }
    }
    if (changed) save();
}

OperationResult Library::addBook(Book book) {
    if (Utils::trim(book.title).empty()) return {false, "Title is required."};
    if (Utils::trim(book.author).empty()) return {false, "Author is required."};
    if (!book.isbn.empty() && book.isbn.size() < 10) return {false, "Invalid ISBN."};
    book.id = nextBookId();
    book.status = BookStatus::Available;
    book.createdAt = Utils::today();
    book.updatedAt = Utils::today();
    data_.books.push_back(book);
    persistAfterChange("Book added: " + book.title);
    return {true, "Book added."};
}

OperationResult Library::editBook(int id, const std::string& field, const std::string& value) {
    auto* book = getBook(id);
    if (!book) return {false, "Book not found."};
    const auto f = Utils::toLower(field);
    if (f == "title") book->title = value;
    else if (f == "author") book->author = value;
    else if (f == "isbn") book->isbn = value;
    else if (f == "publisher") book->publisher = value;
    else if (f == "year") book->publicationYear = std::stoi(value);
    else if (f == "category") book->category = value;
    else if (f == "language") book->language = value;
    else if (f == "pages") book->pages = std::stoi(value);
    else if (f == "location") book->location = value;
    else if (f == "status") book->status = bookStatusFromString(value);
    else return {false, "Unsupported book field."};
    book->updatedAt = Utils::today();
    persistAfterChange("Book edited: " + std::to_string(id));
    return {true, "Book updated."};
}

OperationResult Library::deleteBook(int id) {
    const auto it = std::find_if(data_.books.begin(), data_.books.end(), [id](const Book& b) { return b.id == id; });
    if (it == data_.books.end()) return {false, "Book not found."};
    if (it->status == BookStatus::Borrowed) return {false, "Cannot delete borrowed book."};
    data_.books.erase(it);
    persistAfterChange("Book deleted: " + std::to_string(id));
    return {true, "Book deleted."};
}

OperationResult Library::addReader(Reader reader) {
    if (Utils::trim(reader.firstName).empty() || Utils::trim(reader.lastName).empty()) {
        return {false, "First name and last name are required."};
    }
    if (reader.email.find('@') == std::string::npos) return {false, "Invalid email."};
    reader.id = nextReaderId();
    reader.registrationDate = Utils::today();
    reader.active = true;
    data_.readers.push_back(reader);
    persistAfterChange("Reader added: " + reader.fullName());
    return {true, "Reader added."};
}

OperationResult Library::createLoan(int bookId, int readerId, int days) {
    auto* book = getBook(bookId);
    auto* reader = getReader(readerId);
    if (!book) return {false, "Book not found."};
    if (!reader || !reader->active) return {false, "Reader not found."};
    if (book->status == BookStatus::Borrowed) return {false, "Book already borrowed."};
    if (book->status == BookStatus::Lost) return {false, "Cannot borrow lost book."};
    if (book->status == BookStatus::Maintenance) return {false, "Cannot borrow book in maintenance."};
    if (days <= 0) days = data_.settings.loanPeriodDays;

    const auto now = Utils::today();
    data_.loans.push_back({nextLoanId(), bookId, readerId, now, Utils::addDays(now, days), "", LoanStatus::Active});
    book->status = BookStatus::Borrowed;
    book->updatedAt = now;
    persistAfterChange("Loan created: book " + std::to_string(bookId));
    return {true, "Loan created successfully."};
}

OperationResult Library::returnLoanByBook(int bookId) {
    auto* loan = getActiveLoanForBook(bookId);
    auto* book = getBook(bookId);
    if (!loan || !book) return {false, "Active loan not found."};
    loan->status = LoanStatus::Returned;
    loan->returnDate = Utils::today();
    book->status = BookStatus::Available;
    book->updatedAt = Utils::today();
    persistAfterChange("Loan returned: book " + std::to_string(bookId));
    return {true, "Book returned."};
}

OperationResult Library::createReservation(int bookId, int readerId) {
    const auto book = findBook(bookId);
    const auto reader = findReader(readerId);
    if (!book) return {false, "Book not found."};
    if (!reader || !reader->active) return {false, "Reader not found."};
    const auto duplicate = std::any_of(data_.reservations.begin(), data_.reservations.end(), [&](const Reservation& r) {
        return r.bookId == bookId && r.readerId == readerId;
    });
    if (duplicate) return {false, "Reservation already exists."};
    const auto now = Utils::today();
    data_.reservations.push_back({nextReservationId(), bookId, readerId, now, Utils::addDays(now, 14)});
    auto* editable = getBook(bookId);
    if (editable && editable->status == BookStatus::Available) {
        editable->status = BookStatus::Reserved;
        editable->updatedAt = now;
    }
    persistAfterChange("Reservation created: book " + std::to_string(bookId));
    return {true, "Reservation created."};
}

Optional<Book> Library::findBook(int id) const {
    const auto it = std::find_if(data_.books.begin(), data_.books.end(), [id](const Book& b) { return b.id == id; });
    if (it == data_.books.end()) return Optional<Book>();
    return *it;
}

Optional<Reader> Library::findReader(int id) const {
    const auto it = std::find_if(data_.readers.begin(), data_.readers.end(), [id](const Reader& r) { return r.id == id; });
    if (it == data_.readers.end()) return Optional<Reader>();
    return *it;
}

std::vector<Book> Library::searchBooks(const std::string& query) const {
    return SearchEngine::searchBooks(data_.books, query);
}

std::vector<Book> Library::filterBooks(const std::vector<std::string>& args) const {
    return SearchEngine::filterBooks(data_.books, args);
}

fs::path Library::generateReport() const {
    return ReportGenerator::writeReport(data_.books, data_.readers, data_.loans, data_.reservations);
}

OperationResult Library::exportBooks(const std::string& format, const fs::path& path) const {
    const auto normalized = Utils::toLower(format);
    if (normalized == "csv") fileManager_.exportBooksCsv(data_.books, path);
    else if (normalized == "json") fileManager_.exportBooksJson(data_.books, path);
    else return {false, "Unsupported export format. Use csv or json."};
    logger_.info("Export books: " + path.string());
    return {true, "Export finished."};
}

const std::vector<Book>& Library::books() const { return data_.books; }
const std::vector<Reader>& Library::readers() const { return data_.readers; }
const std::vector<Loan>& Library::loans() const { return data_.loans; }
const std::vector<Reservation>& Library::reservations() const { return data_.reservations; }
const std::vector<Category>& Library::categories() const { return data_.categories; }
const Settings& Library::settings() const { return data_.settings; }

std::string Library::statsText() const {
    return ReportGenerator::buildStatsText(data_.books, data_.readers, data_.loans, data_.reservations);
}

int Library::activeLoanCountForReader(int readerId) const {
    return static_cast<int>(std::count_if(data_.loans.begin(), data_.loans.end(), [readerId](const Loan& l) {
        return l.readerId == readerId && (l.status == LoanStatus::Active || l.status == LoanStatus::Overdue);
    }));
}

int Library::reservationCountForReader(int readerId) const {
    return static_cast<int>(std::count_if(data_.reservations.begin(), data_.reservations.end(), [readerId](const Reservation& r) {
        return r.readerId == readerId;
    }));
}

int Library::loanHistoryCountForReader(int readerId) const {
    return static_cast<int>(std::count_if(data_.loans.begin(), data_.loans.end(), [readerId](const Loan& l) {
        return l.readerId == readerId;
    }));
}

int Library::nextBookId() const {
    int maxId = 0;
    for (const auto& b : data_.books) maxId = std::max(maxId, b.id);
    return maxId + 1;
}

int Library::nextReaderId() const {
    int maxId = 0;
    for (const auto& r : data_.readers) maxId = std::max(maxId, r.id);
    return maxId + 1;
}

int Library::nextLoanId() const {
    int maxId = 0;
    for (const auto& l : data_.loans) maxId = std::max(maxId, l.id);
    return maxId + 1;
}

int Library::nextReservationId() const {
    int maxId = 0;
    for (const auto& r : data_.reservations) maxId = std::max(maxId, r.id);
    return maxId + 1;
}

Book* Library::getBook(int id) {
    const auto it = std::find_if(data_.books.begin(), data_.books.end(), [id](const Book& b) { return b.id == id; });
    return it == data_.books.end() ? nullptr : &(*it);
}

Reader* Library::getReader(int id) {
    const auto it = std::find_if(data_.readers.begin(), data_.readers.end(), [id](const Reader& r) { return r.id == id; });
    return it == data_.readers.end() ? nullptr : &(*it);
}

Loan* Library::getActiveLoanForBook(int bookId) {
    const auto it = std::find_if(data_.loans.begin(), data_.loans.end(), [bookId](const Loan& l) {
        return l.bookId == bookId && (l.status == LoanStatus::Active || l.status == LoanStatus::Overdue);
    });
    return it == data_.loans.end() ? nullptr : &(*it);
}

void Library::persistAfterChange(const std::string& logMessage) {
    save();
    if (data_.settings.autoBackup) fileManager_.backup(data_);
    logger_.info(logMessage);
}
