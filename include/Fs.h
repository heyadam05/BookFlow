#pragma once

#include <string>

#ifdef _WIN32
#include <direct.h>
#include <io.h>
#include <windows.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

namespace fs {
class path {
public:
    path() = default;
    path(const char* value) : value_(value) {}
    path(std::string value) : value_(std::move(value)) {}

    std::string string() const { return value_; }
    const char* c_str() const { return value_.c_str(); }
    bool empty() const { return value_.empty(); }

    path parent_path() const {
        const auto pos = value_.find_last_of("/\\");
        if (pos == std::string::npos) return "";
        return value_.substr(0, pos);
    }

    path operator/(const path& other) const {
        if (value_.empty()) return other;
        if (other.value_.empty()) return *this;
        const char last = value_.back();
        if (last == '/' || last == '\\') return value_ + other.value_;
        return value_ + "/" + other.value_;
    }

private:
    std::string value_;
};

inline bool exists(const path& p) {
#ifdef _WIN32
    return _access(p.c_str(), 0) == 0;
#else
    return access(p.c_str(), F_OK) == 0;
#endif
}

inline void create_directories(const path& p) {
    if (p.empty() || exists(p)) return;
    create_directories(p.parent_path());
#ifdef _WIN32
    _mkdir(p.c_str());
#else
    mkdir(p.c_str(), 0755);
#endif
}

inline bool is_directory(const path& p) {
#ifdef _WIN32
    const DWORD attrs = GetFileAttributesA(p.c_str());
    return attrs != INVALID_FILE_ATTRIBUTES && (attrs & FILE_ATTRIBUTE_DIRECTORY);
#else
    struct stat st {};
    return stat(p.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
#endif
}

inline void remove_all(const path& p) {
    if (!exists(p)) return;
#ifdef _WIN32
    if (is_directory(p)) {
        WIN32_FIND_DATAA data;
        const auto pattern = (p / "*").string();
        HANDLE handle = FindFirstFileA(pattern.c_str(), &data);
        if (handle != INVALID_HANDLE_VALUE) {
            do {
                const std::string name = data.cFileName;
                if (name != "." && name != "..") remove_all(p / name);
            } while (FindNextFileA(handle, &data));
            FindClose(handle);
        }
        RemoveDirectoryA(p.c_str());
    } else {
        DeleteFileA(p.c_str());
    }
#else
    if (is_directory(p)) {
        DIR* dir = opendir(p.c_str());
        if (dir) {
            while (auto* entry = readdir(dir)) {
                const std::string name = entry->d_name;
                if (name != "." && name != "..") remove_all(p / name);
            }
            closedir(dir);
        }
        rmdir(p.c_str());
    } else {
        unlink(p.c_str());
    }
#endif
}
}
