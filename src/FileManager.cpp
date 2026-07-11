#include "FileManager.h"

#include "Utils.h"
#include <fstream>
#include <iomanip>
#include <sstream>

namespace {
std::vector<std::string> readLines(const fs::path& path) {
    std::vector<std::string> lines;
    std::ifstream input(path.string());
    std::string line;
    while (std::getline(input, line)) {
        if (!Utils::trim(line).empty()) lines.push_back(line);
    }
    return lines;
}

void writeLines(const fs::path& path, const std::vector<std::string>& lines) {
    fs::create_directories(path.parent_path());
    std::ofstream output(path.string());
    for (const auto& line : lines) output << line << '\n';
}

int toInt(const std::string& value, int fallback = 0) {
    try { return std::stoi(value); } catch (...) { return fallback; }
}

double toDouble(const std::string& value, double fallback = 0.0) {
    try { return std::stod(value); } catch (...) { return fallback; }
}
}

FileManager::FileManager(fs::path dataDirectory)
    : dataDirectory_(std::move(dataDirectory)) {}

LibraryData FileManager::load() const {
    fs::create_directories(dataDirectory_);
    LibraryData data;

    for (const auto& line : readLines(dataDirectory_ / "books.json")) {
        const auto f = Utils::splitRecord(line);
        if (f.size() < 15) continue;
        Book b;
        b.id = toInt(f[0]);
        b.title = f[1];
        b.author = f[2];
        b.isbn = f[3];
        b.publisher = f[4];
        b.publicationYear = toInt(f[5]);
        b.category = f[6];
        b.language = f[7];
        b.pages = toInt(f[8]);
        b.description = f[9];
        b.location = f[10];
        b.status = bookStatusFromString(f[11]);
        b.createdAt = f[12];
        b.updatedAt = f[13];
        data.books.push_back(b);
    }

    for (const auto& line : readLines(dataDirectory_ / "readers.json")) {
        const auto f = Utils::splitRecord(line);
        if (f.size() < 8) continue;
        Reader r;
        r.id = toInt(f[0]);
        r.firstName = f[1];
        r.lastName = f[2];
        r.email = f[3];
        r.phone = f[4];
        r.address = f[5];
        r.registrationDate = f[6];
        r.active = f[7] == "1";
        data.readers.push_back(r);
    }

    for (const auto& line : readLines(dataDirectory_ / "loans.json")) {
        const auto f = Utils::splitRecord(line);
        if (f.size() < 7) continue;
        Loan l;
        l.id = toInt(f[0]);
        l.bookId = toInt(f[1]);
        l.readerId = toInt(f[2]);
        l.borrowDate = f[3];
        l.dueDate = f[4];
        l.returnDate = f[5];
        l.status = loanStatusFromString(f[6]);
        data.loans.push_back(l);
    }

    for (const auto& line : readLines(dataDirectory_ / "reservations.json")) {
        const auto f = Utils::splitRecord(line);
        if (f.size() < 5) continue;
        data.reservations.push_back({toInt(f[0]), toInt(f[1]), toInt(f[2]), f[3], f[4]});
    }

    for (const auto& line : readLines(dataDirectory_ / "categories.json")) {
        const auto f = Utils::splitRecord(line);
        if (f.size() < 3) continue;
        data.categories.push_back({toInt(f[0]), f[1], f[2]});
    }

    for (const auto& line : readLines(dataDirectory_ / "settings.json")) {
        const auto f = Utils::splitRecord(line);
        if (f.size() < 5) continue;
        data.settings.loanPeriodDays = toInt(f[0], 30);
        data.settings.finePerDay = toDouble(f[1], 0.50);
        data.settings.language = f[2];
        data.settings.dateFormat = f[3];
        data.settings.autoBackup = f[4] == "1";
    }

    return data;
}

void FileManager::save(const LibraryData& data) const {
    std::vector<std::string> lines;
    for (const auto& b : data.books) {
        lines.push_back(Utils::joinRecord({std::to_string(b.id), b.title, b.author, b.isbn, b.publisher,
            std::to_string(b.publicationYear), b.category, b.language, std::to_string(b.pages),
            b.description, b.location, toString(b.status), b.createdAt, b.updatedAt, ""}));
    }
    writeLines(dataDirectory_ / "books.json", lines);

    lines.clear();
    for (const auto& r : data.readers) {
        lines.push_back(Utils::joinRecord({std::to_string(r.id), r.firstName, r.lastName, r.email,
            r.phone, r.address, r.registrationDate, r.active ? "1" : "0"}));
    }
    writeLines(dataDirectory_ / "readers.json", lines);

    lines.clear();
    for (const auto& l : data.loans) {
        lines.push_back(Utils::joinRecord({std::to_string(l.id), std::to_string(l.bookId),
            std::to_string(l.readerId), l.borrowDate, l.dueDate, l.returnDate, toString(l.status)}));
    }
    writeLines(dataDirectory_ / "loans.json", lines);

    lines.clear();
    for (const auto& r : data.reservations) {
        lines.push_back(Utils::joinRecord({std::to_string(r.id), std::to_string(r.bookId),
            std::to_string(r.readerId), r.reservationDate, r.expirationDate}));
    }
    writeLines(dataDirectory_ / "reservations.json", lines);

    lines.clear();
    for (const auto& c : data.categories) {
        lines.push_back(Utils::joinRecord({std::to_string(c.id), c.name, c.description}));
    }
    writeLines(dataDirectory_ / "categories.json", lines);

    writeLines(dataDirectory_ / "settings.json", {Utils::joinRecord({std::to_string(data.settings.loanPeriodDays),
        std::to_string(data.settings.finePerDay), data.settings.language, data.settings.dateFormat,
        data.settings.autoBackup ? "1" : "0"})});
}

void FileManager::backup(const LibraryData& data) const {
    const auto backupDir = dataDirectory_.parent_path() / "backups";
    fs::create_directories(backupDir);
    const auto path = backupDir / ("backup_" + Utils::today() + ".txt");
    std::ofstream output(path.string(), std::ios::app);
    output << "BookFlow backup " << Utils::today() << '\n';
    output << "books=" << data.books.size() << " readers=" << data.readers.size()
           << " loans=" << data.loans.size() << " reservations=" << data.reservations.size() << "\n\n";
}

void FileManager::exportBooksCsv(const std::vector<Book>& books, const fs::path& path) const {
    fs::create_directories(path.parent_path());
    std::ofstream output(path.string());
    output << "id,title,author,isbn,category,status\n";
    for (const auto& b : books) {
        output << b.id << ",\"" << b.title << "\",\"" << b.author << "\",\"" << b.isbn
               << "\",\"" << b.category << "\",\"" << toString(b.status) << "\"\n";
    }
}

void FileManager::exportBooksJson(const std::vector<Book>& books, const fs::path& path) const {
    fs::create_directories(path.parent_path());
    std::ofstream output(path.string());
    output << "[\n";
    for (std::size_t i = 0; i < books.size(); ++i) {
        const auto& b = books[i];
        output << "  {\"id\":" << b.id << ",\"title\":\"" << Utils::jsonEscape(b.title)
               << "\",\"author\":\"" << Utils::jsonEscape(b.author)
               << "\",\"isbn\":\"" << Utils::jsonEscape(b.isbn)
               << "\",\"category\":\"" << Utils::jsonEscape(b.category)
               << "\",\"status\":\"" << toString(b.status) << "\"}";
        output << (i + 1 == books.size() ? "\n" : ",\n");
    }
    output << "]\n";
}
