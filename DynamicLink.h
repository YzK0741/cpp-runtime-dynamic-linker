//
// Created by 小叶 on 25-11-16.
//
#ifndef DYNAMICLINK_H
#define DYNAMICLINK_H

/**
 * @file DynamicLink.h
 * @brief User-friendly interface for dynamic library loading and function calling
 *
 * This header provides a simplified interface for dynamically loading libraries
 * and calling functions with support for hot-reloading and fallback mechanisms.
 */

#include <string>

namespace DynamicLink {
struct FunctionDescriptor;

template<typename T>
concept Callable = std::is_function_v<T> && !std::is_member_function_pointer_v<T>;
/**
* @brief Core class contains safe dynamic function call
*
* @tparam FuncType: the dynamic function's real type
*
* @warning illegal FuncType will cause fatal at the function calling
**/
template<Callable FuncType>
class FunctionWrapper {
public:
    using FuncPointer = std::add_pointer_t<FuncType>;
    explicit FunctionWrapper(const std::string& libName, const std::string& funcName) noexcept;
    FunctionWrapper(FunctionWrapper&&) = default;
    FunctionWrapper& operator=(FunctionWrapper&&) = default;

    /**
    * @brief main function to dynamic calls
    *
    * @param args: the legal arguments the dynamic need, defined by the template FuncType
    * @return defined by the template FuncType
    **/
    template<typename... Args>
    auto operator()(Args... args)
    -> std::invoke_result_t<FuncType*, Args...>;

    /**
     * @brief set the fallback function when the dynamic function accidentally failed
     *
     * @tparam T any callables
     * @param func when can't call the dynamic function will call this function as fallback
     */
    template<typename  T>
    void setFallback(T&& func);

    /**
     * @brief set the check state for the performance
     *      enable checking for the normal but safe performance
     *      disable checking for the high but unsafe performance
     *
     * @param isCheck true for enable checking, false for disable
     *
     * @warning when disable checking you should be responsible for the correct state
     */
    void setCheck(bool isCheck);

    /**
     * @brief for the best performance get the raw for calling
     *
     * @return the raw pointer of the dynamic function
     * @note the pointer will cause error after unload/reload library,
     *      return null when fail to load function
     */
    FuncPointer getRawPointer();
};
/**
 * @brief Core function with creates a type-safe wrapper for a dynamically loaded function
 *
 * @tparam FuncType Function signature (e.g., void(int, const char*))
 * @param lib Library filename (e.g., "libmylib.so", "mydll.dll")
 * @param func Function name (e.g., "my_function")
 * @return FunctionWrapper<FuncType> Callable function object
 *
 * @code
 * // Example usage:
 * auto func = GetFunction<void(int)>("libmathlib.so", "square");
 * func(5);  // Calls the dynamically loaded function
 * @endcode
 */
template<typename FuncType>
FunctionWrapper<FuncType> GetFunction(const std::string& lib, const std::string& func);

/**
 * @brief Preloads a library into memory without resolving any functions
 *
 * Useful for performance optimization when you know you'll need the library later.
 *
 * @param lib Library filename to preload
 *
 * @code
 * // Preload libraries at startup
 * PreloadLibrary("renderer.dll");
 * PreloadLibrary("physics.so");
 * @endcode
 */
void PreloadLibrary(const std::string& lib);

/**
 * @brief Unloads a library from memory
 *
 * All subsequent calls to functions from this library will use fallbacks or throw a std::runtime_error.
 *
 * @param lib Library filename to unload
 */
void UnloadLibrary(const std::string& lib);

/**
 * @brief Hot-reload a library with an another version
 *
 * Switches all function calls from the old library to the new one automatically.
 *
 * @param oldLib Current library filename
 * @param newLib New library filename to replace with
 *
 * @code
 * // Update to new version without restarting
 * ReloadLibrary("plugin_v1.so", "plugin_v2.so");
 * @endcode
 */
void ReloadLibrary(const std::string& oldLib, const std::string& newLib);

/**
 * @brief Preloads a specific function from a library
 *
 * Useful when you want to resolve a function immediately rather than on first call.
 *
 * @param lib Library filename
 * @param func Function name to preload
 */
void PreloadFunction(const std::string& lib, const std::string& func);

} // namespace DynamicLink

#include "DynamicLink.ipp"

#endif // DYNAMICLINK_H
