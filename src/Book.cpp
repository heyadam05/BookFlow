#include "Book.h"

#include "Utils.h"
#include <stdexcept>

std::string toString(BookStatus status) {
    switch (status) {
    case BookStatus::Available: return "Available";
    case BookStatus::Borrowed: return "Borrowed";
    case BookStatus::Reserved: return "Reserved";
    case BookStatus::Lost: return "Lost";
    case BookStatus::Maintenance: return "Maintenance";
    }
    return "Available";
}

BookStatus bookStatusFromString(const std::string& value) {
    const auto normalized = Utils::toLower(Utils::trim(value));
    if (normalized == "available") return BookStatus::Available;
    if (normalized == "borrowed") return BookStatus::Borrowed;
    if (normalized == "reserved") return BookStatus::Reserved;
    if (normalized == "lost") return BookStatus::Lost;
    if (normalized == "maintenance") return BookStatus::Maintenance;
    throw std::invalid_argument("Invalid book status.");
}
