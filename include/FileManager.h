#pragma once

#include "Book.h"
#include "Category.h"
#include "Loan.h"
#include "Reader.h"
#include "Reservation.h"
#include "Settings.h"
#include "Fs.h"
#include <vector>

struct LibraryData {
    std::vector<Book> books;
    std::vector<Reader> readers;
    std::vector<Loan> loans;
    std::vector<Reservation> reservations;
    std::vector<Category> categories;
    Settings settings;
};

class FileManager {
public:
    explicit FileManager(fs::path dataDirectory = "data");

    LibraryData load() const;
    void save(const LibraryData& data) const;
    void backup(const LibraryData& data) const;
    void exportBooksCsv(const std::vector<Book>& books, const fs::path& path) const;
    void exportBooksJson(const std::vector<Book>& books, const fs::path& path) const;

private:
    fs::path dataDirectory_;
};
