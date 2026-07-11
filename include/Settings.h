#pragma once

#include <string>

struct Settings {
    int loanPeriodDays = 30;
    double finePerDay = 0.50;
    std::string language = "English";
    std::string dateFormat = "YYYY-MM-DD";
    bool autoBackup = true;
};
