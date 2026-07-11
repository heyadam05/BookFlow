#pragma once

#include "Fs.h"
#include <string>

class Logger {
public:
    explicit Logger(fs::path logFile = "data/logs.txt");
    void info(const std::string& message) const;

private:
    fs::path logFile_;
};
