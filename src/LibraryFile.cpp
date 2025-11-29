//
// Created by DS on 2025/11/24.

#include <vector>
#include "internal/LibraryFile.h"
#include "internal/Platforms.h"
#ifdef __linux__
const std::string prefix(PREFIX);
#endif


const std::string suffix(SUFFIX);
const std::string systemSearchPath(SYSTEM_PATH);
std::vector<Detail::path> searchPaths;

bool Detail::IsFullName(const std::string_view name) {
#ifdef _WIN32
    return name.ends_with(suffix);
#elif defined __linux__
    return name.starts_with(prefix) && name.ends_with(suffix);
#endif
}

std::string Detail::GetFullName(const std::string &name) {
#ifdef _WIN32
    if (!name.ends_with(suffix))
        return name + suffix;
    return name;

#elif defined __linux__
    auto fullName = name
    if (!name.starts_with(prefix))
        fullName = prefix + fullName;
    if (!name.ends_with(suffix))
        fullname = fullName + suffix;
    return fullName;
#endif
}

void Detail::AddSearchPath(const std::string &path) {
    const Detail::path searchPath(path);
    if (is_directory(searchPath))
        searchPaths.push_back(searchPath);
}

std::optional<Detail::path> Detail::GetLibraryPath(const std::string &name) {
    if (path fullPath("./" + name); is_regular_file(fullPath))
        return fullPath;
    if (path fullPath(systemSearchPath + name); is_regular_file(fullPath))
        return fullPath;
    if (!searchPaths.empty()) {
        for (auto& searchPath : searchPaths) {
            if (path fullPath(searchPath / name); is_regular_file(fullPath))
                return fullPath;
        }
    }
    return std::nullopt;
}

bool Detail::IsValidPath(const std::string &name) {
    if (const path fullPath("." + name); is_regular_file(fullPath))
        return true;
    if (const path fullPath(systemSearchPath + name); is_regular_file(fullPath))
        return true;
    if (!searchPaths.empty()) {
        for (auto& searchPath : searchPaths) {
            if (const path fullPath(searchPath / name); is_regular_file(fullPath))
                return true;
        }
    }
    return false;
}