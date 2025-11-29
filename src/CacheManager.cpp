//

// Created by DS on 2025/11/10.
//

#include <future>
#include <iostream>
#include <mutex>
#include <ranges>
#include <chrono>
#include "internal/CacheManager.h"

LHANDLE
Detail::CacheManager::getLibraryHandle(const std::string &lib) {
    std::shared_lock lock(this->mutex);
    if (const auto cache = this->libraries.find(lib); cache != this->libraries.end())
        return cache->second.handle;
    return nullptr;
}

void
Detail::CacheManager::operator()(const std::string& lib, const LHANDLE handle){
    std::unique_lock lock(this->mutex);
    if (!this->libraries.contains(lib)) {
        this->libraries[lib].handle = handle;
    }
}

void
Detail::CacheManager::operator()(const std::string& lib, const std::string& func, const uintptr_t function) {
    std::unique_lock lock(this->mutex);
    this->libraries[lib].functions[func] = function;
}

bool
Detail::CacheManager::containsLibrary(const std::string& lib) const {
    std::shared_lock lock(this->mutex);
    return this->libraries.contains(lib);
}

bool
Detail::CacheManager::containsFunction(const std::string &lib, const std::string &func){
    std::shared_lock lock(this->mutex);
    if (this->libraries.contains(lib))
        return this->libraries[lib].functions.contains(func);
    return false;
}

Detail::FunctionDescriptor*
Detail::CacheManager::getFunctionDescriptor(const std::string& lib, const std::string& func) {
    std::shared_lock lock(this->mutex);
    return &this->libraries[lib].functions[func];
}

Detail::CacheManager::~CacheManager() {
    for (const auto &[handle, functions] :
        this->libraries | std::views::values) {
        UNLOAD_LIB(handle);
    }
    this->libraries.clear();
}

void Detail::CacheManager::unloadLibrary(const std::string &lib){
    this->doUnloadLibrary(lib);
}


void
Detail::CacheManager::reloadLibrary(const std::string &oldLib, const std::string &newLib) {
    std::unique_lock lock(this->mutex);
    if (!libraries.contains(oldLib)) {
        std::cerr << "Library not found: " << oldLib;
        std::terminate();
    }
    const LHANDLE handle = LoadLibraryWithCheck(newLib);
    this->libraryAlias[oldLib] = newLib;
    for (auto& functionName : this->libraries[oldLib].functions | std::views::keys) {
        if (const auto function = GET_FUNC(handle, functionName.c_str()))
            this->libraries[newLib].functions[functionName] = function;
    }
    this->unloadLibrary(oldLib);
}

bool Detail::CacheManager::isOutOfDate(const std::string & libName) const {
    std::shared_lock lock(this->mutex);
    return this->libraryAlias.contains(libName);
}

std::string Detail::CacheManager::getNewName(const std::string & libName) {
    std::shared_lock lock(this->mutex);
    if (this->libraryAlias.contains(libName))
        return this->libraryAlias[libName];
    return libName;
}

void Detail::CacheManager::doUnloadLibrary(const std::string & lib) {
    std::unique_lock lock(this->mutex);
    for (auto& descriptor :
        this->libraries[lib].functions | std::views::values) {
        descriptor.valid.lock(); // descriptor will be destroyed so avoid the RAII
        descriptor.functionPointer = 0;
        descriptor.valid.unlock();
    }
    UNLOAD_LIB(instance().libraries[lib].handle);
    this->libraries.erase(lib);
    if (this->libraryAlias.contains(lib))
        this->libraryAlias.erase(lib);
}
