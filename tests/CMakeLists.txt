function(FETCH_GTEST)
    # GoogleTest requires at least C++11
    # set(CMAKE_CXX_STANDARD 11)

    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        b796f7d # release-1.13.0
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endfunction()

set(CMAKE_CXX_STANDARD 17)

fetch_gtest()

file(GLOB APP_SOURCES
    "${PROJECT_SOURCE_DIR}/src/pkg/*.cpp"
)

add_executable(test_pool_allocator test.cpp ${APP_SOURCES})
target_link_libraries(test_pool_allocator gtest_main)
target_include_directories(test_pool_allocator
    PRIVATE "${PROJECT_SOURCE_DIR}/include"
    PRIVATE "${PROJECT_SOURCE_DIR}/example"
)

include(GoogleTest)
gtest_discover_tests(test_pool_allocator)
