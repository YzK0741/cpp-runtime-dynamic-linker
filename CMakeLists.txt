cmake_minimum_required(VERSION 3.31.6)
project(DynamicLink VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

add_library(DynamicLink STATIC src/DynamicLinkImpl.cpp
        src/FunctionDescriptor.cpp
        src/CacheManager.cpp
        src/LibraryFile.cpp
)

target_include_directories(DynamicLink PRIVATE include)
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

find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_PROJECT_NAME "DynamicLink")
    set(DOXYGEN_PROJECT_BRIEF "cpp-runtime-dynamic-linker")
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/docs)
    set(DOXYGEN_GENERATE_HTML YES)
    set(DOXYGEN_GENERATE_LATEX NO)

    if(DOXYGEN_DOT_FOUND)
        set(DOXYGEN_HAVE_DOT YES)
        set(DOXYGEN_CALL_GRAPH YES)
        set(DOXYGEN_CALLER_GRAPH YES)
    else()
        set(DOXYGEN_HAVE_DOT NO)
        message(WARNING "Graphviz not found, diagrams will not be generated")
    endif()

    doxygen_add_docs(documentation
            ${CMAKE_SOURCE_DIR}/include
            ${CMAKE_SOURCE_DIR}/src
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    )
endif()