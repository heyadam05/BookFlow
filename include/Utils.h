#pragma once

#include <string>
#include <vector>

namespace Utils {
std::string trim(const std::string& value);
std::string toLower(std::string value);
std::vector<std::string> splitCommand(const std::string& line);
std::string today();
std::string addDays(const std::string& isoDate, int days);
int daysBetween(const std::string& fromIsoDate, const std::string& toIsoDate);
bool isPositiveNumber(const std::string& value);
std::string escapeField(const std::string& value);
std::string unescapeField(const std::string& value);
std::vector<std::string> splitRecord(const std::string& line);
std::string joinRecord(const std::vector<std::string>& fields);
std::string jsonEscape(const std::string& value);
}
