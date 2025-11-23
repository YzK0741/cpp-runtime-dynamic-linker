#include <iostream>
#include "DynamicLinkImpl.h"
#include "CacheManager.h"

#define instance Detail::CacheManager::instance()

Detail::FunctionDescriptor*
Detail::GetFunctionImpl(const std::string &lib, const std::string &func) {
    if (instance.containsLibrary(lib)) {
        if (instance.containsFunction(lib, func)) {
            auto descriptor = instance.getFunctionDescriptor(lib, func);
            if (descriptor->functionPointer == 0) {
                std::cerr << "bad functon" << func << "in lib" << lib;
                std::terminate();
            }
            return instance.getFunctionDescriptor(lib, func);
        }

        const LHANDLE handle = instance.getLibraryHandle(lib);
        const auto function = GET_FUNC(handle, func.c_str());
        if (!function) {
            std::cerr << "bad functon" << func << "in lib" << lib;
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