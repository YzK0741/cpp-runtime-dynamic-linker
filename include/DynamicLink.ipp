//
// Created by DS on 2025/11/21.
//

#ifndef DYNAMICLINK_DYNAMICLINK_IPP
#define DYNAMICLINK_DYNAMICLINK_IPP

#include <functional>
#include <iostream>
#include <memory>
#include <shared_mutex>
#include <string>
#include "DynamicLinkImpl.h"

namespace DynamicLink {


    template<typename T>
    concept Callable = std::is_function_v<T> && !std::is_member_function_pointer_v<T>;
    template<Callable FuncType>
    class FunctionWrapper {
    public:
        using FuncPointer = std::add_pointer_t<FuncType>;
        explicit FunctionWrapper(const std::string& libName, const std::string& funcName) noexcept {
            this->descriptor = Detail::GetFunctionImpl(libName, funcName);
            if (!this->descriptor || !this->descriptor->functionPointer) {
                std::cerr << "dynamic function not found";
                std::terminate();
            }
            this->libName = libName;
            this->funcName = funcName;
        }

        FunctionWrapper(FunctionWrapper&&) = default;
        FunctionWrapper& operator=(FunctionWrapper&&) = default;

        template<typename... Args>
        auto operator()(Args... args)
        -> std::invoke_result_t<FuncType*, Args...> {
            using ReturnType = std::invoke_result_t<FuncType*, Args...>;

            if (this->check){
                if (Detail::DescriptorShouldUpdate(libName)) {
                    this->libName = Detail::GetActualLibraryName(this->libName);
                    this->descriptor = Detail::GetFunctionImpl(libName, funcName);
                }

                if (!this->descriptor->functionPointer) {
                    if (this->fallback){
                        std::cerr << "invalid dynamic function call: library has unloaded with no fallback";
                        std::terminate();
                    }
                } else {
                    std::cerr << "invalid dynamic function call: library has unloaded with no fallback";
                    std::terminate();
                }
            } // check

            std::shared_lock<std::shared_mutex> lock(this->descriptor->valid);

            if constexpr (std::is_void_v<ReturnType>) {
                if (!this->descriptor->functionPointer) {
                    if (this->fallback) {
                        std::invoke(
                            std::forward<std::function<FuncType>>(this->fallback),
                            std::forward<Args>(args)...
                            );
                    } else {
                        std:: cerr << "invalid dynamic function call: library has unloaded with no fallback";
                        std::terminate();
                    }
                } else {
                    std::invoke(
                       reinterpret_cast<FuncPointer>(this->descriptor->functionPointer),
                       std::forward<Args>(args)...
                       );
                }
                return;
            }
            else {
                ReturnType res;
                if (!this->descriptor->functionPointer) {
                    if (this->fallback) {
                        res = std::invoke(
                            std::forward<std::function<FuncType>>(this->fallback),
                            std::forward<Args>(args)...
                            );
                    } else {
                        std:: cerr << "invalid dynamic function call: library has unloaded with no fallback";
                        std::terminate();
                    }
                } else {
                    res = std::invoke(
                       reinterpret_cast<FuncPointer>(this->descriptor->functionPointer),
                       std::forward<Args>(args)...
                       );
                }
                return res;
            }
        }

        template<typename  T>
        void setFallback(T&& func) {
            this->fallback = std::make_unique<std::function<FuncType>>(std::forward<T>(func));
        }

        void setCheck(bool isCheck) {
            this->check = isCheck;
        }

        FuncPointer getRawPointer() {
            return reinterpret_cast<FuncPointer>(this->descriptor->functionPointer);
        }

    private:
        Detail::FunctionDescriptor* descriptor{nullptr};//< save the dynamic function's address
        std::string libName;
        std::string funcName;
        std::unique_ptr<std::function<FuncType>> fallback; //< when can't call dynamic function,
                                                           //call this one (all callable objects)
        bool check{true};
    };
    template<typename FuncType>
    FunctionWrapper<FuncType> GetFunction(const std::string& lib, const std::string& func){
        FunctionWrapper<FuncType> function(lib, func);
        return function;
    }
}

#endif //DYNAMICLINK_DYNAMICLINK_IPP
