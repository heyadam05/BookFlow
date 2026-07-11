#pragma once

#include "Models.h"
#include <string>

struct Loan {
    int id = 0;
    int bookId = 0;
    int readerId = 0;
    std::string borrowDate;
    std::string dueDate;
    std::string returnDate;
    LoanStatus status = LoanStatus::Active;
};
