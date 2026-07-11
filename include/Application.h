#pragma once

#include "Library.h"
#include <string>

class Application {
public:
    explicit Application(std::string dataDirectory = "data");
    int run();

private:
    void printWelcome() const;
    void printHelp() const;
    bool execute(const std::string& line);
    void handleBook(const std::vector<std::string>& args);
    void handleReader(const std::vector<std::string>& args);
    void handleLoan(const std::vector<std::string>& args);
    void handleReservation(const std::vector<std::string>& args);
    void printBooks(const std::vector<Book>& books) const;
    void printReaders() const;
    void printBook(const Book& book) const;
    void printReader(const Reader& reader) const;
    std::string prompt(const std::string& label) const;
    int promptInt(const std::string& label, int fallback = 0) const;

    Library library_;
};
