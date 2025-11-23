//
// Created by DS on 2025/11/10.
//

#ifndef DYNAMICLINK_CACHEMANAGER_H
#define DYNAMICLINK_CACHEMANAGER_H


#include <unordered_map>
#include <shared_mutex>
#include "DynamicLinkImpl.h"

//Single instance mode class
namespace Detail {
    class CacheManager {
    public:
        static CacheManager& instance() {
            static CacheManager manager;
            return manager;
        }
        void operator()(const std::string&, LHANDLE);
        void operator()(const std::string& lib, const std::string& func, uintptr_t function);
        bool containsLibrary(const std::string&) const;
        bool containsFunction(const std::string& lib, const std::string& func);
        FunctionDescriptor* getFunctionDescriptor(const std::string& lib, const std::string& func);
        LHANDLE getLibraryHandle(const std::string&);
        void unloadLibrary(const std::string&);
        void reloadLibrary(const std::string&, const std::string&);
        bool isOutOfDate(const std::string&) const;
        std::string getNewName(const std::string &);

    private:
        CacheManager() = default;
        ~CacheManager();
        struct LibraryCache {
            LHANDLE handle;
            std::unordered_map<std::string, FunctionDescriptor> functions;
        };
        std::unordered_map<std::string, LibraryCache> libraries{};
        using NewName = std::string; using OldName = std::string;
        std::unordered_map<OldName, NewName> libraryAlias{};
        mutable std::shared_mutex mutex{};
        void doUnloadLibrary(const std::string&);
    };
}


#endif //DYNAMICLINK_CACHEMANAGER_H