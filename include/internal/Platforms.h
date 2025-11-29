//
// Created by DS on 2025/11/21.
//

#ifndef DYNAMICLINK_PLATFORMS_H
#define DYNAMICLINK_PLATFORMS_H

#ifdef _WIN32

#include <windows.h>
#define LOAD_LIB(x) LoadLibrary(x)
#define UNLOAD_LIB(x) FreeLibrary(x)
#define GET_FUNC(x, y) reinterpret_cast<uintptr_t>(GetProcAddress(x, y))
#define LHANDLE HMODULE
#define SUFFIX ".lib"
#define SYSTEM_PATH "C:/Windows/System32/"


//Get system error
inline std::string GetWindowsError() {
    const DWORD errorCode = GetLastError();
    if (errorCode == 0) {return "No error";}

    LPSTR buffer = nullptr;
    size_t size = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, errorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPSTR>(&buffer), 0, nullptr);

    std::string message(buffer, size);
    LocalFree(buffer);

    if (!message.empty() && message.back() == '\n') {
        message.pop_back();
        if (!message.empty() && message.back() == '\r') {
            message.pop_back();
        }
    }

    return message;
}

#define GET_ERROR() GetWindowsError()
#elif defined(__linux__)

#include <dlfcn.h>
int GetFlag();
void SetFlag(int);
#define LOAD_LIB(x) dlopen(x, GetFlag())
#define UNLOAD_LIB(x) dlclose(x)
#define GET_FUNC(x, y) reinterpret_cast<uintptr_t>(dlsym(x, y))
#define LHANDLE void*
#define PREFIX "lib"
#define SUFFIX ".so"


//Get system error
inline std::string GetPosixError() {
    return dlerror() ?: "Unknown error";
}

#define GET_ERROR() GetPosixError()

#endif



#endif //DYNAMICLINK_PLATFORMS_H