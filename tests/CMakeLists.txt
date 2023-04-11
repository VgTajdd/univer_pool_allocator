# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        703bd9caab50b139428cea1aaff9974ebee5742e # release-1.10.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB APP_SOURCES
    "${PROJECT_SOURCE_DIR}/src/pkg/*.cpp"
)

add_executable(test_template test.cpp ${APP_SOURCES})
target_link_libraries(test_template gtest_main)
target_include_directories(test_template
    PRIVATE "${PROJECT_SOURCE_DIR}/include"
)

include(GoogleTest)
gtest_discover_tests(test_template)
