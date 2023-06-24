cmake_minimum_required(VERSION 3.16.0)
project(univer_pool_allocator VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)

file(GLOB SOURCES
    "src/pkg/*.cpp"
    "src/*.cpp"
)

add_executable(app ${SOURCES})
target_include_directories(app PUBLIC include)

# Tests.
enable_testing()
add_subdirectory(tests EXCLUDE_FROM_ALL)
