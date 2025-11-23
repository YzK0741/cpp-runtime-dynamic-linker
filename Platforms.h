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

#elif defined(__linux__)

#include <dlfcn.h>
static int LDFLAGS = RTLD_LAZY;
#define LOAD_LIB(x) dlopen(x, LDFLAGS)
#define UNLOAD_LIB(x) dlclose(x)
#define GET_FUNC(x, y) reinterpret_cast<uintptr_t>(dlsym(x, y))
#define LHANDLE void*

#endif



#endif //DYNAMICLINK_PLATFORMS_H