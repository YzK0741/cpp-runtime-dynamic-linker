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

#elif defined(__linux__)

#include <dlfcn.h>
int GetFlag();
void SetFlag(int);
#define LOAD_LIB(x) dlopen(x, LDFLAGS)
#define UNLOAD_LIB(x) dlclose(x)
#define GET_FUNC(x, y) reinterpret_cast<uintptr_t>(dlsym(x, y))
#define LHANDLE void*
#define PREFIX "lib"
#define SUFFIX ".so"

#endif



#endif //DYNAMICLINK_PLATFORMS_H