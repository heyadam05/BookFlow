#pragma once

#include <string>

struct Reservation {
    int id = 0;
    int bookId = 0;
    int readerId = 0;
    std::string reservationDate;
    std::string expirationDate;
};
