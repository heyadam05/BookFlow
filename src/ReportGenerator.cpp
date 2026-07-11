#include "ReportGenerator.h"

#include "Models.h"
#include "Utils.h"
#include <fstream>
#include <sstream>

std::string ReportGenerator::buildStatsText(const std::vector<Book>& books,
                                            const std::vector<Reader>& readers,
                                            const std::vector<Loan>& loans,
                                            const std::vector<Reservation>& reservations) {
    int borrowed = 0;
    int available = 0;
    int overdue = 0;
    for (const auto& book : books) {
        if (book.status == BookStatus::Borrowed) ++borrowed;
        if (book.status == BookStatus::Available) ++available;
    }
    for (const auto& loan : loans) {
        if (loan.status == LoanStatus::Overdue) ++overdue;
    }

    std::ostringstream out;
    out << "Books: " << books.size() << '\n'
        << "Readers: " << readers.size() << '\n'
        << "Loans: " << loans.size() << '\n'
        << "Borrowed: " << borrowed << '\n'
        << "Available: " << available << '\n'
        << "Reservations: " << reservations.size() << '\n'
        << "Overdue: " << overdue << '\n';
    return out.str();
}

fs::path ReportGenerator::writeReport(const std::vector<Book>& books,
                                                   const std::vector<Reader>& readers,
                                                   const std::vector<Loan>& loans,
                                                   const std::vector<Reservation>& reservations,
                                                   const fs::path& reportsDirectory) {
    fs::create_directories(reportsDirectory);
    const auto path = reportsDirectory / ("library_report_" + Utils::today() + ".txt");
    std::ofstream output(path.string());
    output << "BookFlow Library Report\n";
    output << "Generated: " << Utils::today() << "\n\n";
    output << buildStatsText(books, readers, loans, reservations);
    return path;
}
