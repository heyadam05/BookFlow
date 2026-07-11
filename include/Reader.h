#pragma once

#include <string>

struct Reader {
    int id = 0;
    std::string firstName;
    std::string lastName;
    std::string email;
    std::string phone;
    std::string address;
    std::string registrationDate;
    bool active = true;

    std::string fullName() const;
};
