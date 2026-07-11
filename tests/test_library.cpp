#include "Library.h"

#include <cassert>
#include <iostream>

namespace {
void clean(const fs::path& path) {
    if (fs::exists(path)) fs::remove_all(path);
}
}

int main() {
    const fs::path dataDir = "test_data";
    clean(dataDir);
    clean("backups");

    Library library(dataDir);
    library.load();

    Book book;
    book.title = "Clean Code";
    book.author = "Robert C. Martin";
    book.isbn = "9780132350884";
    book.publisher = "Prentice Hall";
    book.publicationYear = 2008;
    book.category = "Programming";
    book.language = "English";
    book.pages = 464;
    book.location = "A1";
    assert(library.addBook(book).success);
    assert(library.books().size() == 1);

    Reader reader;
    reader.firstName = "John";
    reader.lastName = "Smith";
    reader.email = "john@example.com";
    reader.phone = "+421900123456";
    reader.address = "Poprad";
    assert(library.addReader(reader).success);
    assert(library.readers().size() == 1);

    assert(library.createLoan(1, 1, 30).success);
    assert(library.findBook(1)->status == BookStatus::Borrowed);
    assert(!library.deleteBook(1).success);
    assert(!library.createLoan(1, 1, 30).success);
    assert(library.returnLoanByBook(1).success);
    assert(library.findBook(1)->status == BookStatus::Available);

    assert(library.createReservation(1, 1).success);
    assert(library.findBook(1)->status == BookStatus::Reserved);
    assert(!library.createReservation(1, 1).success);

    auto search = library.searchBooks("clean");
    assert(search.size() == 1);
    auto filtered = library.filterBooks({"category", "Programming"});
    assert(filtered.size() == 1);

    auto report = library.generateReport();
    assert(fs::exists(report));
    assert(library.exportBooks("csv", "test_data/books_export.csv").success);
    assert(library.exportBooks("json", "test_data/books_export.json").success);
    assert(fs::exists("test_data/books_export.csv"));
    assert(fs::exists("test_data/books_export.json"));

    Library reloaded(dataDir);
    reloaded.load();
    assert(reloaded.books().size() == 1);
    assert(reloaded.readers().size() == 1);
    assert(reloaded.loans().size() == 1);
    assert(reloaded.reservations().size() == 1);

    clean(dataDir);
    std::cout << "All BookFlow tests passed.\n";
    return 0;
}
