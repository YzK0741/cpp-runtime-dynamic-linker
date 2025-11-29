//
// Created by DS on 2025/11/24.
//

#ifndef DYNAMICLINK_LIBRARYFILE_H
#define DYNAMICLINK_LIBRARYFILE_H

#include <filesystem>
#include <string>
#include <string_view>
#include <optional>

namespace Detail {
    using path = std::filesystem::path;
    bool IsFullName(std::string_view name);
    std::string GetFullName(const std::string&);
    void AddSearchPath(const std::string&);
    std::optional<path> GetLibraryPath(const std::string& name);
    bool IsValidPath(const std::string &name);
} // Detail

#endif //DYNAMICLINK_LIBRARYFILE_H