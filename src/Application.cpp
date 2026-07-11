#include "Application.h"

#include "Utils.h"
#include <iomanip>
#include <iostream>

Application::Application(std::string dataDirectory) : library_(std::move(dataDirectory)) {}

int Application::run() {
    library_.load();
    printWelcome();
    std::string line;
    while (true) {
        std::cout << "\n> ";
        if (!std::getline(std::cin, line)) break;
        if (!execute(line)) break;
    }
    return 0;
}

void Application::printWelcome() const {
    std::cout << "===========================================\n";
    std::cout << "BookFlow\nLibrary Management System\nVersion 1.0\n";
    std::cout << "===========================================\n\n";
    std::cout << "Database loaded successfully.\n\n";
    std::cout << "Books: " << library_.books().size() << '\n';
    std::cout << "Readers: " << library_.readers().size() << '\n';
    std::cout << "Active loans: ";
    int active = 0;
    for (const auto& loan : library_.loans()) {
        if (loan.status == LoanStatus::Active || loan.status == LoanStatus::Overdue) ++active;
    }
    std::cout << active << "\n\nType \"help\" for available commands.\n";
}

void Application::printHelp() const {
    std::cout << "Commands:\n";
    std::cout << "  help\n";
    std::cout << "  book add | book list | book show <id> | book edit <id> <field> <value> | book delete <id>\n";
    std::cout << "  reader add | reader list | reader show <id>\n";
    std::cout << "  loan create | loan return <bookId>\n";
    std::cout << "  reservation create\n";
    std::cout << "  search <text>\n";
    std::cout << "  filter category <name> | filter author <name> | filter available | filter year >2020\n";
    std::cout << "  stats | report | export csv <path> | export json <path>\n";
    std::cout << "  exit\n";
}

bool Application::execute(const std::string& line) {
    const auto args = Utils::splitCommand(line);
    if (args.empty()) return true;
    const auto command = Utils::toLower(args[0]);
    if (command == "exit" || command == "quit") return false;
    if (command == "help") printHelp();
    else if (command == "book") handleBook(args);
    else if (command == "reader") handleReader(args);
    else if (command == "loan") handleLoan(args);
    else if (command == "reservation") handleReservation(args);
    else if (command == "search") {
        const auto pos = line.find(' ');
        printBooks(library_.searchBooks(pos == std::string::npos ? "" : line.substr(pos + 1)));
    } else if (command == "filter") {
        printBooks(library_.filterBooks(std::vector<std::string>(args.begin() + 1, args.end())));
    } else if (command == "stats") {
        std::cout << library_.statsText();
    } else if (command == "report") {
        std::cout << "Report generated: " << library_.generateReport().string() << '\n';
    } else if (command == "export" && args.size() >= 3) {
        const auto result = library_.exportBooks(args[1], args[2]);
        std::cout << result.message << '\n';
    } else {
        std::cout << "Unknown command. Type help.\n";
    }
    return true;
}

void Application::handleBook(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << "Missing book command.\n";
        return;
    }
    const auto action = Utils::toLower(args[1]);
    if (action == "add") {
        Book book;
        book.title = prompt("Title");
        book.author = prompt("Author");
        book.isbn = prompt("ISBN");
        book.publisher = prompt("Publisher");
        book.publicationYear = promptInt("Year");
        book.category = prompt("Category");
        book.language = prompt("Language");
        book.pages = promptInt("Pages");
        book.location = prompt("Shelf");
        book.description = prompt("Description");
        std::cout << library_.addBook(book).message << '\n';
    } else if (action == "list") {
        printBooks(library_.books());
    } else if (action == "show" && args.size() >= 3) {
        const auto book = library_.findBook(std::stoi(args[2]));
        if (book) printBook(*book);
        else std::cout << "Book not found.\n";
    } else if (action == "edit" && args.size() >= 5) {
        std::string value = args[4];
        for (std::size_t i = 5; i < args.size(); ++i) value += " " + args[i];
        std::cout << library_.editBook(std::stoi(args[2]), args[3], value).message << '\n';
    } else if (action == "delete" && args.size() >= 3) {
        std::cout << library_.deleteBook(std::stoi(args[2])).message << '\n';
    } else {
        std::cout << "Invalid book command.\n";
    }
}

void Application::handleReader(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << "Missing reader command.\n";
        return;
    }
    const auto action = Utils::toLower(args[1]);
    if (action == "add") {
        Reader reader;
        reader.firstName = prompt("First name");
        reader.lastName = prompt("Last name");
        reader.email = prompt("Email");
        reader.phone = prompt("Phone");
        reader.address = prompt("Address");
        std::cout << library_.addReader(reader).message << '\n';
    } else if (action == "list") {
        printReaders();
    } else if (action == "show" && args.size() >= 3) {
        const auto reader = library_.findReader(std::stoi(args[2]));
        if (reader) printReader(*reader);
        else std::cout << "Reader not found.\n";
    } else {
        std::cout << "Invalid reader command.\n";
    }
}

void Application::handleLoan(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << "Missing loan command.\n";
        return;
    }
    const auto action = Utils::toLower(args[1]);
    if (action == "create") {
        const int bookId = promptInt("Book ID");
        const int readerId = promptInt("Reader ID");
        const int days = promptInt("Loan period (days)", library_.settings().loanPeriodDays);
        std::cout << library_.createLoan(bookId, readerId, days).message << '\n';
    } else if (action == "return" && args.size() >= 3) {
        std::cout << library_.returnLoanByBook(std::stoi(args[2])).message << '\n';
    } else {
        std::cout << "Invalid loan command.\n";
    }
}

void Application::handleReservation(const std::vector<std::string>& args) {
    if (args.size() >= 2 && Utils::toLower(args[1]) == "create") {
        const int bookId = promptInt("Book ID");
        const int readerId = promptInt("Reader ID");
        std::cout << library_.createReservation(bookId, readerId).message << '\n';
    } else {
        std::cout << "Invalid reservation command.\n";
    }
}

void Application::printBooks(const std::vector<Book>& books) const {
    std::cout << std::left << std::setw(6) << "ID" << std::setw(28) << "Title"
              << std::setw(24) << "Author" << "Status\n";
    for (const auto& b : books) {
        std::cout << std::setw(6) << b.id << std::setw(28) << b.title.substr(0, 27)
                  << std::setw(24) << b.author.substr(0, 23) << toString(b.status) << '\n';
    }
}

void Application::printReaders() const {
    std::cout << std::left << std::setw(6) << "ID" << std::setw(28) << "Name" << "Borrowed books\n";
    for (const auto& r : library_.readers()) {
        std::cout << std::setw(6) << r.id << std::setw(28) << r.fullName().substr(0, 27)
                  << library_.activeLoanCountForReader(r.id) << '\n';
    }
}

void Application::printBook(const Book& book) const {
    std::cout << "ID: " << book.id << '\n'
              << "Title: " << book.title << '\n'
              << "Author: " << book.author << '\n'
              << "ISBN: " << book.isbn << '\n'
              << "Publisher: " << book.publisher << '\n'
              << "Year: " << book.publicationYear << '\n'
              << "Category: " << book.category << '\n'
              << "Language: " << book.language << '\n'
              << "Pages: " << book.pages << '\n'
              << "Location: " << book.location << '\n'
              << "Status: " << toString(book.status) << '\n';
}

void Application::printReader(const Reader& reader) const {
    std::cout << "Name: " << reader.fullName() << '\n'
              << "Email: " << reader.email << '\n'
              << "Phone: " << reader.phone << '\n'
              << "Registered: " << reader.registrationDate << '\n'
              << "Current loans: " << library_.activeLoanCountForReader(reader.id) << '\n'
              << "Reservations: " << library_.reservationCountForReader(reader.id) << '\n'
              << "Loan history: " << library_.loanHistoryCountForReader(reader.id) << '\n';
}

std::string Application::prompt(const std::string& label) const {
    std::cout << label << ": ";
    std::string value;
    std::getline(std::cin, value);
    return Utils::trim(value);
}

int Application::promptInt(const std::string& label, int fallback) const {
    const auto value = prompt(label);
    if (value.empty()) return fallback;
    try { return std::stoi(value); } catch (...) { return fallback; }
}
