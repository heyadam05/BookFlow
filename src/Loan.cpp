#include "Loan.h"

#include "Utils.h"
#include <stdexcept>

std::string toString(LoanStatus status) {
    switch (status) {
    case LoanStatus::Active: return "Active";
    case LoanStatus::Returned: return "Returned";
    case LoanStatus::Overdue: return "Overdue";
    }
    return "Active";
}

LoanStatus loanStatusFromString(const std::string& value) {
    const auto normalized = Utils::toLower(Utils::trim(value));
    if (normalized == "active") return LoanStatus::Active;
    if (normalized == "returned") return LoanStatus::Returned;
    if (normalized == "overdue") return LoanStatus::Overdue;
    throw std::invalid_argument("Invalid loan status.");
}
