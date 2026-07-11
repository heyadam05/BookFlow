#pragma once

#include <string>

enum class BookStatus { Available, Borrowed, Reserved, Lost, Maintenance };
enum class LoanStatus { Active, Returned, Overdue };

std::string toString(BookStatus status);
std::string toString(LoanStatus status);
BookStatus bookStatusFromString(const std::string& value);
LoanStatus loanStatusFromString(const std::string& value);
