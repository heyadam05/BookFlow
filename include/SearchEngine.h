#pragma once

#include "Book.h"
#include <string>
#include <vector>

class SearchEngine {
public:
    static std::vector<Book> searchBooks(const std::vector<Book>& books, const std::string& query);
    static std::vector<Book> filterBooks(const std::vector<Book>& books, const std::vector<std::string>& args);
};
