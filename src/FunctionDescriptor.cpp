//
// Created by DS on 2025/11/10.
//

//***** FunctionDescriptor's Implement *****

#include "internal//DynamicLinkImpl.h"

Detail::FunctionDescriptor::FunctionDescriptor() {
    this->functionPointer = 0;
}

Detail::FunctionDescriptor::FunctionDescriptor(const uintptr_t funcPtr){
    this->functionPointer = funcPtr;
}

Detail::FunctionDescriptor::FunctionDescriptor(const FunctionDescriptor &other)  noexcept {
    this->functionPointer = other.functionPointer;
}

Detail::FunctionDescriptor &
Detail::FunctionDescriptor::operator=(FunctionDescriptor &&other) noexcept {
    this->functionPointer = other.functionPointer;
    return *this;
}
Detail::FunctionDescriptor &
Detail::FunctionDescriptor::operator=(const FunctionDescriptor& other) noexcept {
    this->functionPointer = other.functionPointer;
    return *this;
}

Detail::FunctionDescriptor &Detail::FunctionDescriptor::operator=(const uintptr_t funcPtr) {
    this->functionPointer = funcPtr;
    return *this;
}