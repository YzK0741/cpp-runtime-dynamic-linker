# DynamicLink

A lightweight, high-performance C++ dynamic library loader with hot-reloading support. Designed for maximum performance and minimal footprint.

## 🚀 Features

- **🔥 Hot Reloading** - Swap libraries at runtime without restarting
- **🎯 Type Safety** - Template-based function wrappers with compile-time checks
- **⚡ High Performance** - Optimized caching with optional safety checks
- **🛡️ Thread Safe** - Full thread support with shared mutex protection
- **🌍 Cross Platform** - Windows and Linux support
- **🔄 Fallback Support** - Graceful degradation with custom fallback functions

## 📋 Requirements

- C++23 compatible compiler
- CMake 3.31.6+
- Windows (MSVC) or Linux (GCC/Clang)

## 🛠️ Installation

### Using CMake

```cmake
include(FetchContent)
FetchContent_Declare(
    DynamicLink
    GIT_REPOSITORY https://github.com/YzK0741/DynamicLink.git
    GIT_TAG        main
)
FetchContent_MakeAvailable(DynamicLink)

target_link_libraries(your_target PRIVATE DynamicLink)
```

### Manual Build

```bash
git clone https://github.com/YzK0741/DynamicLink.git
cd DynamicLink
mkdir build && cd build
cmake ..
make
sudo make install  # Optional
```

## 🎯 Quick Start

```cpp
#include <DynamicLink.h>

// Define function signature
using AddFunc = int(int, int);

int main() {
    // Load function from shared library
    auto add = DynamicLink::GetFunction<AddFunc>("libmath.so", "add");
    
    // Call dynamically loaded function
    int result = add(5, 3);  // Returns 8
    
    // Set fallback for error handling
    add.setFallback([](int a, int b) {
        std::cout << "Using fallback function\n";
        return a + b;
    });
    
    return 0;
}
```

## 📚 Usage Examples

### Basic Function Loading

```cpp
// Load and call a simple function
auto printf_func = DynamicLink::GetFunction<int(const char*, ...)>("libc.so.6", "printf");
printf_func("Hello from dynamic function!\n");
```

### Hot Reloading

```cpp
// Update library without restarting
DynamicLink::ReloadLibrary("plugin_v1.so", "plugin_v2.so");

// All subsequent calls automatically use the new version
auto new_func = DynamicLink::GetFunction<void()>("plugin_v2.so", "new_feature");
new_func();
```

### Performance Optimization

```cpp
auto fast_func = DynamicLink::GetFunction<void()>("libfast.so", "critical_function");
fast_func.setCheck(false);  // Disable safety checks for maximum performance

// Get raw pointer for direct calls (unsafe but fastest)
auto raw_ptr = fast_func.getRawPointer();
if (raw_ptr) {
    raw_ptr();  // Direct call, no overhead
}
```

### Preloading for Better Performance

```cpp
// Preload libraries at startup
DynamicLink::PreloadLibrary("renderer.dll");
DynamicLink::PreloadLibrary("physics.so");

// Preload specific functions
DynamicLink::PreloadFunction("renderer.dll", "render_frame");
DynamicLink::PreloadFunction("physics.so", "update_physics");

// Later usage has zero loading overhead
auto render = DynamicLink::GetFunction<void()>("renderer.dll", "render_frame");
auto update = DynamicLink::GetFunction<void(float)>("physics.so", "update_physics");
```

## 🔧 Advanced Features

### Custom Error Handling

```cpp
auto risky_func = DynamicLink::GetFunction<bool()>("unstable_lib.so", "risky_operation");

// Set comprehensive fallback
risky_func.setFallback([]() -> bool {
    std::cerr << "Dynamic function failed, using safe implementation\n";
    return safe_implementation();
});

// Disable checking if you manage library lifecycle manually
risky_func.setCheck(false);
```

### Library Management

```cpp
// Manual library control
DynamicLink::PreloadLibrary("optional.dll");

// ... later in code
if (need_optional_features) {
    auto opt_func = DynamicLink::GetFunction<void()>("optional.dll", "feature");
    opt_func();
}

// Cleanup when done
DynamicLink::UnloadLibrary("optional.dll");
```

## 🏗️ Building from Source

```bash
# Clone repository
git clone https://github.com/YzK0741/DynamicLink.git
cd DynamicLink

# Configure with optimal flags
cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-fno-rtti -fno-exceptions"

# Build
cmake --build build

```
## 🎯 API Reference

### Core Functions

- `GetFunction<FuncType>(library, function)` - Main function loader
- `PreloadLibrary(library)` - Load library without resolving symbols
- `PreloadFunction(library, function)` - Pre-resolve specific function
- `UnloadLibrary(library)` - Unload library from memory
- `ReloadLibrary(old_library, new_library)` - Hot-reload library

### FunctionWrapper Methods

- `operator()` - Call wrapped function
- `setFallback(func)` - Set fallback function
- `setCheck(enable)` - Enable/disable safety checks
- `getRawPointer()` - Get raw function pointer with type

## 🤝 Contributing

We welcome contributions! Please feel free to submit pull requests, report bugs, or suggest new features.

1. Fork the repository
2. Create your feature branch
3. Commit your changes
4. Push to the branch 
5. Open a Pull Request

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- Inspired by modern C++ best practices
- Built with performance as the primary focus
- Cross-platform compatibility as a core design goal

---

**⭐ Star this repo if you find it useful!**
