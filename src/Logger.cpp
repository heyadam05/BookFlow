#include "Logger.h"

#include "Utils.h"
#include <fstream>

Logger::Logger(fs::path logFile) : logFile_(std::move(logFile)) {}

void Logger::info(const std::string& message) const {
    fs::create_directories(logFile_.parent_path());
    std::ofstream output(logFile_.string(), std::ios::app);
    output << Utils::today() << " - " << message << '\n';
}
