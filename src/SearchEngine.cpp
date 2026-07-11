#include "SearchEngine.h"

#include "Utils.h"

std::vector<Book> SearchEngine::searchBooks(const std::vector<Book>& books, const std::string& query) {
    const auto q = Utils::toLower(query);
    std::vector<Book> results;
    for (const auto& book : books) {
        const auto text = Utils::toLower(book.title + " " + book.author + " " + book.isbn + " " +
                                         book.publisher + " " + book.category);
        if (text.find(q) != std::string::npos) results.push_back(book);
    }
    return results;
}

std::vector<Book> SearchEngine::filterBooks(const std::vector<Book>& books, const std::vector<std::string>& args) {
    std::vector<Book> results;
    if (args.empty()) return results;
    const auto key = Utils::toLower(args[0]);
    const auto value = args.size() > 1 ? Utils::toLower(args[1]) : "";

    for (const auto& book : books) {
        bool match = false;
        if (key == "available") match = book.status == BookStatus::Available;
        else if (key == "borrowed") match = book.status == BookStatus::Borrowed;
        else if (key == "reserved") match = book.status == BookStatus::Reserved;
        else if (key == "category" && args.size() > 1) match = Utils::toLower(book.category).find(value) != std::string::npos;
        else if (key == "author" && args.size() > 1) match = Utils::toLower(book.author).find(value) != std::string::npos;
        else if (key == "year" && args.size() > 1) {
            const auto expr = args[1];
            if (expr.size() > 1 && (expr[0] == '>' || expr[0] == '<')) {
                const int year = std::stoi(expr.substr(1));
                match = expr[0] == '>' ? book.publicationYear > year : book.publicationYear < year;
            } else {
                match = book.publicationYear == std::stoi(expr);
            }
        }
        if (match) results.push_back(book);
    }
    return results;
}
