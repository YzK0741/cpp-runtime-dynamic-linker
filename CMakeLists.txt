cmake_minimum_required(VERSION 3.31.6)
project(DynamicLink VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

add_library(DynamicLink STATIC DynamicLinkImpl.cpp
        FunctionDescriptor.cpp
        CacheManager.cpp
)

target_compile_options(DynamicLink PRIVATE
    -fno-rtti
    -fno-exceptions
)

set_target_properties(DynamicLink PROPERTIES
    OUTPUT_NAME "DynamicLink"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

install(TARGETS DynamicLink
    EXPORT DynamicLinkTargets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(FILES
        DynamicLink.h
        DynamicLink.ipp
    DESTINATION include
)

install(EXPORT DynamicLinkTargets
    FILE DynamicLinkConfig.cmake
    NAMESPACE DynamicLink::
    DESTINATION lib/cmake/DynamicLink
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    DynamicLinkConfigVersion.cmake
    VERSION ${PACKAGE_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/DynamicLinkConfigVersion.cmake
    DESTINATION lib/cmake/DynamicLink
)