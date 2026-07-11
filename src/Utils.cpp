#include "Utils.h"

#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace {
std::tm parseDate(const std::string& value) {
    std::tm tm{};
    std::istringstream stream(value);
    stream >> std::get_time(&tm, "%Y-%m-%d");
    tm.tm_hour = 12;
    return tm;
}
}

namespace Utils {
std::string trim(const std::string& value) {
    const auto start = value.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    const auto end = value.find_last_not_of(" \t\r\n");
    return value.substr(start, end - start + 1);
}

std::string toLower(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) {
        return static_cast<char>(std::tolower(c));
    });
    return value;
}

std::vector<std::string> splitCommand(const std::string& line) {
    std::istringstream stream(line);
    std::vector<std::string> parts;
    std::string part;
    while (stream >> part) parts.push_back(part);
    return parts;
}

std::string today() {
    const auto now = std::chrono::system_clock::now();
    const auto time = std::chrono::system_clock::to_time_t(now);
    std::tm tm{};
    tm = *std::localtime(&time);
    std::ostringstream output;
    output << std::put_time(&tm, "%Y-%m-%d");
    return output.str();
}

std::string addDays(const std::string& isoDate, int days) {
    auto tm = parseDate(isoDate);
    std::time_t time = std::mktime(&tm) + static_cast<long long>(days) * 24 * 60 * 60;
    std::tm result{};
    result = *std::localtime(&time);
    std::ostringstream output;
    output << std::put_time(&result, "%Y-%m-%d");
    return output.str();
}

int daysBetween(const std::string& fromIsoDate, const std::string& toIsoDate) {
    auto from = parseDate(fromIsoDate);
    auto to = parseDate(toIsoDate);
    const auto fromTime = std::mktime(&from);
    const auto toTime = std::mktime(&to);
    return static_cast<int>((toTime - fromTime) / (24 * 60 * 60));
}

bool isPositiveNumber(const std::string& value) {
    return !value.empty() && std::all_of(value.begin(), value.end(), [](unsigned char c) {
        return std::isdigit(c);
    }) && std::stoi(value) > 0;
}

std::string escapeField(const std::string& value) {
    std::string escaped;
    for (char c : value) {
        if (c == '\\' || c == '|') escaped.push_back('\\');
        if (c == '\n' || c == '\r') {
            escaped += "\\n";
        } else {
            escaped.push_back(c);
        }
    }
    return escaped;
}

std::string unescapeField(const std::string& value) {
    std::string output;
    bool escaped = false;
    for (char c : value) {
        if (escaped) {
            output.push_back(c == 'n' ? '\n' : c);
            escaped = false;
        } else if (c == '\\') {
            escaped = true;
        } else {
            output.push_back(c);
        }
    }
    return output;
}

std::vector<std::string> splitRecord(const std::string& line) {
    std::vector<std::string> fields;
    std::string current;
    bool escaped = false;
    for (char c : line) {
        if (escaped) {
            current.push_back('\\');
            current.push_back(c);
            escaped = false;
        } else if (c == '\\') {
            escaped = true;
        } else if (c == '|') {
            fields.push_back(unescapeField(current));
            current.clear();
        } else {
            current.push_back(c);
        }
    }
    if (escaped) current.push_back('\\');
    fields.push_back(unescapeField(current));
    return fields;
}

std::string joinRecord(const std::vector<std::string>& fields) {
    std::ostringstream output;
    for (std::size_t i = 0; i < fields.size(); ++i) {
        if (i > 0) output << '|';
        output << escapeField(fields[i]);
    }
    return output.str();
}

std::string jsonEscape(const std::string& value) {
    std::string escaped;
    for (char c : value) {
        if (c == '"' || c == '\\') escaped.push_back('\\');
        if (c == '\n') escaped += "\\n";
        else escaped.push_back(c);
    }
    return escaped;
}
}
