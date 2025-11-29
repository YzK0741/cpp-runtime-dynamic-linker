#include <iostream>
#include "internal/DynamicLinkImpl.h"
#include "internal/CacheManager.h"
#include "internal/Platforms.h"

#define instance Detail::CacheManager::instance()

#ifdef __linux__

int ldFlag = RTLD_LAZY;
int GetFlag(){return ldFlag;}
void SetFlag(int flag){ldFlag = flag;}

#endif

Detail::FunctionDescriptor*
Detail::GetFunctionImpl(const std::string &lib, const std::string &func) {
    if (instance.containsLibrary(lib)) {
        if (instance.containsFunction(lib, func)) {
            if (const auto descriptor = instance.getFunctionDescriptor(lib, func); descriptor->functionPointer == 0) {
                std::cerr << "bad function " << func << "in lib " << lib;
                std::terminate();
            }
            return instance.getFunctionDescriptor(lib, func);
        }

        const LHANDLE handle = instance.getLibraryHandle(lib);
        const auto function = GET_FUNC(handle, func.c_str());
        if (!function) {
            std::cerr << "bad function " << func << "in lib " << lib;
            std::terminate();
        }
        instance(lib, func, function);
        return instance.getFunctionDescriptor(lib, func);
    }

    const LHANDLE handle = LoadLibraryWithCheck(lib);
    const auto function = GET_FUNC(handle, func.c_str());
    instance(lib, handle);
    instance(lib, func, function);
    return instance.getFunctionDescriptor(lib, func);
}

LHANDLE Detail::LoadLibraryWithCheck(const std::string &lib) {
    std::string libraryName = lib;
    if (!IsFullName(libraryName)) {
        libraryName = GetFullName(libraryName);
    }
    if (!IsValidPath(libraryName)) {
        std::cerr << std::format("lib {} does not exist", lib);
        std::terminate();
    }
    const LHANDLE handle = LOAD_LIB(libraryName.c_str());
    if (!handle) {
        std::cerr << "failed at loading" << libraryName;
        std::terminate();
    }
    return handle;
}


void DynamicLink::PreloadLibrary(const std::string &lib) {
    if (!instance.containsLibrary(lib)) {
        const LHANDLE handle = Detail::LoadLibraryWithCheck(lib);
        instance(lib, handle);
    }
}

void DynamicLink::UnloadLibrary(const std::string &lib) {
    instance.unloadLibrary(lib);
}

void DynamicLink::ReloadLibrary(const std::string &oldLib, const std::string &newLib) {
    instance.reloadLibrary(oldLib, newLib);
}

void DynamicLink::PreloadFunction(const std::string &lib, const std::string &func) {
    if (!instance.containsFunction(lib, func)) {
        const LHANDLE handle = instance.getLibraryHandle(lib);
        instance(lib, func, GET_FUNC(handle, func.c_str()));
    }
}

bool Detail::DescriptorShouldUpdate(const std::string &libName) {
    return instance.isOutOfDate(libName);
}

std::string Detail::GetActualLibraryName(const std::string &oldLib) {
    return instance.getNewName(oldLib);
}