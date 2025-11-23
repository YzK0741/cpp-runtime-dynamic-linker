#ifndef DYNAMICLINK_LIBRARY_H
#define DYNAMICLINK_LIBRARY_H

#include <string>
#include <memory>
#include <shared_mutex>
#include <iostream>
#include "Platforms.h"
namespace Detail {
    struct FunctionDescriptor {
        FunctionDescriptor();
        explicit FunctionDescriptor(uintptr_t);
        FunctionDescriptor(const FunctionDescriptor&) noexcept ;
        FunctionDescriptor& operator=(FunctionDescriptor&&) noexcept ;
        FunctionDescriptor& operator=(const FunctionDescriptor&) noexcept ;
        FunctionDescriptor& operator=(uintptr_t);

        uintptr_t functionPointer;
        std::shared_mutex valid;
    };

    FunctionDescriptor* GetFunctionImpl(const std::string &lib, const std::string &func);
    bool DescriptorShouldUpdate(const std::string& libName);
    std::string GetActualLibraryName(const std::string& oldLib);

    inline LHANDLE LoadLibraryWithCheck(const std::string& lib) {
        const LHANDLE handle = LOAD_LIB(lib.c_str());
        if (!handle) {
            std::cerr << "failed at loading" << lib;
            std::terminate();
        }
        return handle;
    }
}

namespace DynamicLink {
    void PreloadLibrary(const std::string& lib);
    void UnloadLibrary(const std::string& lib);
    void ReloadLibrary(const std::string& oldLib, const std::string& newLib);
    void PreloadFunction(const std::string& lib, const std::string& func);
}
#endif // DYNAMICLINK_LIBRARY_H
