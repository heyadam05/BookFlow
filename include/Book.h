#pragma once

#include "Models.h"
#include <string>

struct Book {
    int id = 0;
    std::string title;
    std::string author;
    std::string isbn;
    std::string publisher;
    int publicationYear = 0;
    std::string category;
    std::string language;
    int pages = 0;
    std::string description;
    std::string location;
    BookStatus status = BookStatus::Available;
    std::string createdAt;
    std::string updatedAt;
};
