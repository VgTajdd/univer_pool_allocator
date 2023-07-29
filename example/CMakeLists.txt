file(GLOB SOURCES
    "*.cpp"
)

add_executable(app ${SOURCES})
target_link_libraries(app univer_pool_allocator)

# Defines.
list(APPEND DEBUG_DEFINES DEBUG)
target_compile_definitions(app PRIVATE "$<$<CONFIG:Debug>:${DEBUG_DEFINES}>")
