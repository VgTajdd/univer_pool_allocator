cmake_minimum_required(VERSION 3.16.0)
project(univer_pool_allocator VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES
    "src/*.cpp"
)

add_library(univer_pool_allocator ${SOURCES})
target_include_directories(univer_pool_allocator PUBLIC include)

# Defines.
list(APPEND DEBUG_DEFINES DEBUG)
target_compile_definitions(univer_pool_allocator PRIVATE "$<$<CONFIG:Debug>:${DEBUG_DEFINES}>")

# Options.
option(UVR_POOL_ALLOCATOR_BUILD_TESTS "Build tests" ON)
option(UVR_POOL_ALLOCATOR_BUILD_EXAMPLES "Build tests" ON)

# Tests.
if(UVR_POOL_ALLOCATOR_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Example.
if(UVR_POOL_ALLOCATOR_BUILD_EXAMPLES)
    add_subdirectory(example)
endif()
