#pragma once

#include "Book.h"
#include "Loan.h"
#include "Reader.h"
#include "Reservation.h"
#include "Fs.h"
#include <string>
#include <vector>

class ReportGenerator {
public:
    static std::string buildStatsText(const std::vector<Book>& books,
                                      const std::vector<Reader>& readers,
                                      const std::vector<Loan>& loans,
                                      const std::vector<Reservation>& reservations);

    static fs::path writeReport(const std::vector<Book>& books,
                                             const std::vector<Reader>& readers,
                                             const std::vector<Loan>& loans,
                                             const std::vector<Reservation>& reservations,
                                             const fs::path& reportsDirectory = "reports");
};
